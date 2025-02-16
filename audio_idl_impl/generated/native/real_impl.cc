/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define _HAS_STD_BYTE 0

#include <algorithm>
#include <array>
#include <chrono>
#include <future>
#include <thread>

#include "audio.h"
#include "Serializers.h"
#include "interop-logging.h"
#include "arkoala-macros.h"
#include "tree.h"
#include "logging.h"
#include "dynamic-loader.h"

#undef max

// For logging we use operations exposed via interop, SetLoggerSymbol() is called
// when library is loaded.
const GroupLogger* loggerInstance = GetDefaultLogger();

const GroupLogger* GetDummyLogger() {
    return loggerInstance;
}

void SetDummyLogger(const GroupLogger* logger) {
    loggerInstance = logger;
}

void startGroupedLog(int kind) {
    GetDummyLogger()->startGroupedLog(kind);
}
void stopGroupedLog(int kind) {
    GetDummyLogger()->stopGroupedLog(kind);
}
const char* getGroupedLog(int kind) {
    return GetDummyLogger()->getGroupedLog(kind);
}
int needGroupedLog(int kind) {
    return GetDummyLogger()->needGroupedLog(kind);
}
void appendGroupedLog(int kind, const std::string& str) {
    GetDummyLogger()->appendGroupedLog(kind, str.c_str());
}

void dummyClassFinalizer(KNativePointer* ptr) {
    char hex[20];
    std::snprintf(hex, sizeof(hex), "0x%llx", (long long)ptr);
    string out("dummyClassFinalizer(");
    out.append(hex);
    out.append(")");
    appendGroupedLog(1, out);
}

namespace TreeNodeDelays {

void busyWait(Ark_Int64 nsDelay) {
    if (nsDelay <= 0) {
        return;
    }
    using namespace std::chrono;
    auto start = steady_clock::now();
    auto now = start;
    auto deadline = now + nanoseconds(nsDelay);
    std::array<char, 8> buf;
    for (; now < deadline; now = steady_clock::now()) {
        auto nsNow = now.time_since_epoch().count();
        buf = { static_cast<char>(nsNow%100 + 20), 19, 18, 17, 16, 15, 14, static_cast<char>(nsNow%12) };
        for (int i = 0; i < 200; i++) {
            std::next_permutation(buf.begin(), buf.end());
        }
    }
    //ARKOALA_LOG("Requested wait %f ms, actual %f ms\n", nsDelay/1000000.0f, (now - start).count()/1000000.0f);
}

const int MAX_NODE_TYPE = 200;
std::array<Ark_Int64, MAX_NODE_TYPE> createNodeDelay = {};\
std::array<Ark_Int64, MAX_NODE_TYPE> measureNodeDelay = {};
std::array<Ark_Int64, MAX_NODE_TYPE> layoutNodeDelay = {};
std::array<Ark_Int64, MAX_NODE_TYPE> drawNodeDelay = {};

void CheckType(GENERATED_Ark_NodeType type) {
    if (type >= MAX_NODE_TYPE) {
        LOG("Error: GENERATED_Ark_NodeType value is too big, change MAX_NODE_TYPE accordingly");
        throw "Error";
    }
}

void SetCreateNodeDelay(GENERATED_Ark_NodeType type, Ark_Int64 nanoseconds) {
    CheckType(type);
    createNodeDelay[type] = nanoseconds;
}

void SetMeasureNodeDelay(GENERATED_Ark_NodeType type, Ark_Int64 nanoseconds) {
    CheckType(type);
    measureNodeDelay[type] = nanoseconds;
}

void SetLayoutNodeDelay(GENERATED_Ark_NodeType type, Ark_Int64 nanoseconds) {
    CheckType(type);
    layoutNodeDelay[type] = nanoseconds;
}

void SetDrawNodeDelay(GENERATED_Ark_NodeType type, Ark_Int64 nanoseconds) {
    CheckType(type);
    drawNodeDelay[type] = nanoseconds;
}

}

inline Ark_NodeHandle AsNodeHandle(TreeNode* node) {
    return reinterpret_cast<Ark_NodeHandle>(node);
}

template<typename From>
constexpr TreeNode *AsNode(From ptr) {
    return reinterpret_cast<TreeNode *>(ptr);
}

void DumpTree(TreeNode *node, Ark_Int32 indent) {
    ARKOALA_LOG("%s[%s: %d]\n", string(indent * 2, ' ').c_str(), node->namePtr(), node->id());
    for (auto child: *node->children()) {
        if (child)
            DumpTree(child, indent + 1);
    }
}

// TODO: remove in favour of callbackCallerInstance!
GENERATED_Ark_APICallbackMethod *callbacks = nullptr;

int TreeNode::_globalId = 1;
string TreeNode::_noAttribute;

Ark_Float32 parseLength(Ark_Float32 parentValue, Ark_Float32 value, Ark_Int32 unit) {
    switch (unit) {
        //PX
        case 0: {
            const Ark_Float32 scale = 1; // TODO: need getting current device scale
            return value * scale;
        }
        //PERCENTAGE
        case 3: {
            return parentValue / 100 * value;
        }
        default:
            // VP, FP, LPX, UndefinedDimensionUnit: TODO: parse properly this units
            return value;
    }
}

void align(TreeNode *child, Ark_Float32 width, Ark_Float32 height, Ark_Float32* args) {
    switch (child->alignment) {
        case 0: { // Alignment.TopStart
            break;
        }
        case 3: { // Alignment.Start
            args[1] += (height - child->measureResult[1]) / 2;
            break;
        }
        case 6: { // Alignment.BottomStart
            args[1] += height - child->measureResult[1];
            break;
        }
        case 1: { // Alignment.Top
            args[0] += (width - child->measureResult[0]) / 2;
            break;
        }
        case 4: { // Alignment.Center
            args[0] += (width - child->measureResult[0]) / 2;
            args[1] += (height - child->measureResult[1]) / 2;
            break;
        }
        case 7: { // Alignment.Bottom
            args[0] += (width - child->measureResult[0]) / 2;
            args[1] += height - child->measureResult[1];
            break;
        }
        case 2: { // Alignment.TopEnd
            args[0] += width - child->measureResult[0];
            break;
        }
        case 5: { // Alignment.End
            args[0] += width - child->measureResult[0];
            args[1] += (height - child->measureResult[1]) / 2;
            break;
        }
        case 8: { // Alignment.BottomEnd
            args[0] += width - child->measureResult[0];
            args[1] += height - child->measureResult[1];
            break;
        }
    }
}

GENERATED_Ark_EventCallbackArg arg(Ark_Float32 f32) {
    GENERATED_Ark_EventCallbackArg result;
    result.f32 = f32;
    return result;
}

GENERATED_Ark_EventCallbackArg arg(Ark_Int32 i32) {
    GENERATED_Ark_EventCallbackArg result;
    result.i32 = i32;
    return result;
}

float TreeNode::measure(Ark_VMContext vmContext, float* data) {
    TreeNodeDelays::busyWait(TreeNodeDelays::measureNodeDelay[_customIntData]);

    Ark_Float32 minWidth = data[0];
    Ark_Float32 minHeight = data[1];
    Ark_Float32 maxWidth = data[2];
    Ark_Float32 maxHeight = data[3];
    if (_flags & Ark_APINodeFlags::GENERATED_CUSTOM_MEASURE) {
        GENERATED_Ark_EventCallbackArg args[] = { arg(Ark_APICustomOp::GENERATED_MEASURE), arg(minWidth), arg(minHeight), arg(maxWidth), arg(maxHeight) };
        callbacks->CallInt(vmContext, customId(), 5, &args[0]);
        _width = args[1].f32;
        _height = args[2].f32;
        return 0;
    }

    const Ark_Float32 constraintWidth = data[0];
    const Ark_Float32 constraintHeight = data[1];

    _width = parseLength(constraintWidth, dimensionWidth.value, dimensionWidth.unit);
    _height = parseLength(constraintHeight, dimensionHeight.value, dimensionHeight.unit);

    Ark_Float32 itData[] = { minWidth, minHeight, minHeight, maxHeight };
    if (dimensionWidth.unit != UndefinedDimensionUnit) {
        itData[0] = _width;
    }
    if (dimensionHeight.unit != UndefinedDimensionUnit) {
        itData[1] = _height;
    }

    const bool isWidthWrapped = dimensionWidth.unit == UndefinedDimensionUnit;
    const bool isHeightWrapped = dimensionHeight.unit == UndefinedDimensionUnit;

    for (auto* it: *children()) {
        it->measure(vmContext, &itData[0] );
        if (isWidthWrapped) {
            _width = std::max(_width, itData[0]);
        }
        if (isHeightWrapped) {
            _height = std::max(_height, itData[1]);
        }
    }

    data[0] = _width;
    data[1] = _height;

    measureResult = &data[0];

    // TODO: use return flag for dirty bits propagation.
    return 0;
}

Ark_CanvasHandle getCanvas(TreeNode* node) {
    // TODO: real canvas.
    return reinterpret_cast<Ark_CanvasHandle>(0x123456789aLL);
}

float TreeNode::layout(Ark_VMContext vmContext, float* data) {
    TreeNodeDelays::busyWait(TreeNodeDelays::layoutNodeDelay[_customIntData]);

    if (_flags & Ark_APINodeFlags::GENERATED_CUSTOM_LAYOUT) {
        GENERATED_Ark_EventCallbackArg args[] = { arg(Ark_APICustomOp::GENERATED_LAYOUT), arg(0.0f), arg(0.0f), arg(0.0f), arg(0.0f) };
        callbacks->CallInt(vmContext, customId(), 5, &args[0]);
        return 0;
    }

    _x = data[0];
    _y = data[1];

    for (auto* it: *children()) {
        Ark_Float32 itData[] = { data[0], data[1], data[2], data[3] };
        align(it, _width, _height, &itData[0]);
        it->layout(vmContext, &itData[0]);
    }

    layoutResult = &data[0];

    // TODO: use return flag for dirty bits propagation.
    return 0;
}

float TreeNode::draw(Ark_VMContext vmContext, float* data) {
    TreeNodeDelays::busyWait(TreeNodeDelays::drawNodeDelay[_customIntData]);
    if (_flags & Ark_APINodeFlags::GENERATED_CUSTOM_DRAW) {
        uintptr_t canvas = reinterpret_cast<uintptr_t>(getCanvas(this));
        GENERATED_Ark_EventCallbackArg args[] = {
            arg(Ark_APICustomOp::GENERATED_DRAW),
            arg((Ark_Int32)(canvas & 0xffffffff)),
            arg((Ark_Int32)((canvas >> 32) & 0xffffffff)),
            arg(data[0]), arg(data[1]), arg(data[2]), arg(data[3])
        };
        callbacks->CallInt(vmContext, customId(), 7, &args[0]);
        return 0;
    }
    for (auto* it: *children()) {
        Ark_Float32 itData[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        it->draw(vmContext, &itData[0]);
    }
    return 0;
}

void TreeNode::setMeasureWidthValue(float value) {
    if (measureResult != nullptr) measureResult[0] = value;
    _width = value;
}

float TreeNode::getMeasureWidthValue() {
    return (measureResult == nullptr) ? 0 : measureResult[0];
}

void TreeNode::setMeasureHeightValue(float value) {
    if (measureResult != nullptr) measureResult[1] = value;
    _height = value;
}

float TreeNode::getMeasureHeightValue() {
    return (measureResult == nullptr) ? 0 : measureResult[1];
}

void TreeNode::setXValue(float value) {
    if (layoutResult != nullptr) layoutResult[0] = value;
    _x = value;
}

float TreeNode::getXValue() {
    return (layoutResult == nullptr) ? 0 : layoutResult[0];
}

void TreeNode::setYValue(float value) {
    if (layoutResult != nullptr) layoutResult[1] = value;
    _y = value;
}

float TreeNode::getYValue() {
    return (layoutResult == nullptr) ? 0 : layoutResult[1];
}

namespace OHOS::Ace::NG {

namespace GeneratedBridge {

Ark_NodeHandle CreateNode(GENERATED_Ark_NodeType type, Ark_Int32 id, Ark_Int32 flags) {
    TreeNodeDelays::CheckType(type);
    TreeNodeDelays::busyWait(TreeNodeDelays::createNodeDelay[type]);
    TreeNode *node = new TreeNode("node", id, flags);
    node->setCustomIntData(type);
    Ark_NodeHandle result = AsNodeHandle(node);

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_NodeHandle peer" + std::to_string((uintptr_t)result) + " = GetBasicNodeApi()->createNode(GENERATED_Ark_NodeType("
            + std::to_string(type) + "), " + std::to_string(id) + ", " + std::to_string(flags) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return result;
    }
    string out("createNode(");
    WriteToString(&out, (Ark_Int32)type);
    out.append(", ");
    WriteToString(&out, id);
    out.append(", ");
    WriteToString(&out, flags);
    out.append(")");
    appendGroupedLog(1, out);
    return result;
}
}

namespace GeneratedApiImpl {

static int res_num = 0;

void SetCallbackMethod(GENERATED_Ark_APICallbackMethod* method) {
    callbacks = method;
}

Ark_Float32 GetDensity(Ark_Int32 deviceId) {
    Ark_Float32 result = 1.0f;

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("getDensity(");
    WriteToString(&out, deviceId);
    out.append(")");
    appendGroupedLog(1, out);

    return result;
}

Ark_Float32 GetFontScale(Ark_Int32 deviceId) {
    Ark_Float32 result = 1.0f;

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("getFontScale(");
    WriteToString(&out, deviceId);
    out.append(")");
    appendGroupedLog(1, out);

    return result;
}

Ark_Float32 GetDesignWidthScale(Ark_Int32 deviceId) {
    Ark_Float32 result = 1.0f;

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("getDesignWidthScale(");
    WriteToString(&out, deviceId);
    out.append(")");
    appendGroupedLog(1, out);

    return result;
}

Ark_NodeHandle GetNodeByViewStack() {
    Ark_NodeHandle result = (Ark_NodeHandle) 234;
    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_NodeHandle peer" + std::to_string((uintptr_t)result) + " = GetBasicNodeApi()->getNodeByViewStack();\n");
        appendGroupedLog(2, _logData);
    }
    if (!needGroupedLog(1)) {
        return result;
    }
    string out("getNodeByViewStack()");
    appendGroupedLog(1, out);
    return result;
}

void DisposeNode(Ark_NodeHandle node) {
    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  GetBasicNodeApi()->disposeNode(peer" + std::to_string((uintptr_t)node) + ");\n");
        appendGroupedLog(2, _logData);
    }
    if (needGroupedLog(1)) {
        string out("disposeNode(");
        WriteToString(&out, node);
        out.append(")");
        appendGroupedLog(1, out);
    }
    AsNode(node)->dispose();
}

void DumpTreeNode(Ark_NodeHandle node) {
    DumpTree(AsNode(node), 0);

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  GetBasicNodeApi()->dumpTreeNode(peer" + std::to_string((uintptr_t)node) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return;
    }

    string out("dumpTreeNode(");
    WriteToString(&out, node);
    out.append(")");
    appendGroupedLog(1, out);
}

Ark_Int32 AddChild(Ark_NodeHandle parent, Ark_NodeHandle child) {
    int result = AsNode(parent)->addChild(AsNode(child));

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_Int32 res" + std::to_string(res_num++) + " = GetBasicNodeApi()->addChild(peer"
            + std::to_string((uintptr_t)parent) + ", peer" + std::to_string((uintptr_t)child) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("addChild(");
    WriteToString(&out, parent);
    out.append(", ");
    WriteToString(&out, child);
    out.append(")");
    appendGroupedLog(1, out);

    // TODO: implement test
    return result;
}

void RemoveChild(Ark_NodeHandle parent, Ark_NodeHandle child) {
    TreeNode *parentPtr = reinterpret_cast<TreeNode *>(parent);
    TreeNode *childPtr = reinterpret_cast<TreeNode *>(child);
    parentPtr->removeChild(childPtr);

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  GetBasicNodeApi()->removeChild(peer"
            + std::to_string((uintptr_t)parent) + ", peer" + std::to_string((uintptr_t)child) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return;
    }

    string out("removeChild(");
    WriteToString(&out, parent);
    out.append(", ");
    WriteToString(&out, child);
    out.append(")");
    appendGroupedLog(1, out);
}

Ark_Int32 InsertChildAfter(Ark_NodeHandle parent, Ark_NodeHandle child, Ark_NodeHandle sibling) {
    int result = AsNode(parent)->insertChildAfter(AsNode(child), AsNode(sibling));

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_Int32 res" + std::to_string(res_num++) + " = GetBasicNodeApi()->insertChildAfter(peer"
            + std::to_string((uintptr_t)parent) + ", peer" + std::to_string((uintptr_t)child)
            + ", peer" + std::to_string((uintptr_t)sibling) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("insertChildAfter(");
    WriteToString(&out, parent);
    out.append(", ");
    WriteToString(&out, child);
    out.append(", ");
    WriteToString(&out, sibling);
    out.append(")");
    appendGroupedLog(1, out);
    return result;
}

Ark_Int32 InsertChildBefore(Ark_NodeHandle parent, Ark_NodeHandle child, Ark_NodeHandle sibling) {
    int result = AsNode(parent)->insertChildBefore(AsNode(child), AsNode(sibling));

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_Int32 res" + std::to_string(res_num++) + " = GetBasicNodeApi()->insertChildBefore(peer"
            + std::to_string((uintptr_t)parent) + ", peer" + std::to_string((uintptr_t)child)
            + ", peer" + std::to_string((uintptr_t)sibling) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("insertChildBefore(");
    WriteToString(&out, parent);
    out.append(", ");
    WriteToString(&out, child);
    out.append(", ");
    WriteToString(&out, sibling);
    out.append(")");
    appendGroupedLog(1, out);
    return result;
}

Ark_Int32 InsertChildAt(Ark_NodeHandle parent, Ark_NodeHandle child, Ark_Int32 position) {
    int result = AsNode(parent)->insertChildAt(AsNode(child), position);

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_Int32 res" + std::to_string(res_num++) + " = GetBasicNodeApi()->insertChildAt(peer"
            + std::to_string((uintptr_t)parent) + ", peer" + std::to_string((uintptr_t)child)
            + ", " + std::to_string(position) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("insertChildAt(");
    WriteToString(&out, parent);
    out.append(", ");
    WriteToString(&out, child);
    out.append(", ");
    WriteToString(&out, position);
    out.append(")");
    appendGroupedLog(1, out);
    return result;
}

void ApplyModifierFinish(Ark_NodeHandle node) {

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  GetBasicNodeApi()->applyModifierFinish(peer" + std::to_string((uintptr_t)node) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return;
    }
    string out("applyModifierFinish(");
    WriteToString(&out, node);
    out.append(")");
    appendGroupedLog(1, out);
}

void MarkDirty(Ark_NodeHandle node, Ark_UInt32 flag) {

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  GetBasicNodeApi()->markDirty(peer" + std::to_string((uintptr_t)node) + ", " + std::to_string(flag) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return;
    }
    string out("markDirty(");
    WriteToString(&out, node);
    out.append(", ");
    WriteToString(&out, flag);
    out.append(")");
    appendGroupedLog(1, out);
}

Ark_Boolean IsBuilderNode(Ark_NodeHandle node) {
    Ark_Boolean result = true;

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_Boolean res" + std::to_string(res_num++) + " = GetBasicNodeApi()->isBuilderNode(peer"
            + std::to_string((uintptr_t)node) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return result;
    }
    string out("isBuilderNode(");
    WriteToString(&out, node);
    out.append(")");
    appendGroupedLog(1, out);
    return result;
}

Ark_Float32 ConvertLengthMetricsUnit(Ark_Float32 value, Ark_Int32 originUnit, Ark_Int32 targetUnit) {
    Ark_Float32 result = value * originUnit;

    if (needGroupedLog(2)) {
        std::string _logData;
        _logData.append("  Ark_Float32 res" + std::to_string(res_num++) + " = GetBasicNodeApi()->convertLengthMetricsUnit("
            + std::to_string(value) + ", " + std::to_string(originUnit) + ", " + std::to_string(targetUnit) + ");\n");
        appendGroupedLog(2, _logData);
    }

    if (!needGroupedLog(1)) {
        return result;
    }

    string out("convertLengthMetricsUnit(");
    WriteToString(&out, value);
    out.append(", ");
    WriteToString(&out, originUnit);
    out.append(", ");
    WriteToString(&out, targetUnit);
    out.append(")");
    appendGroupedLog(1, out);
    return result;
}

void EmitOnClick(Ark_NativePointer node, Ark_ClickEvent event) {
    auto frameNode = AsNode(node);
    frameNode->callClickEvent(event);
}

void SetCustomMethodFlag(Ark_NodeHandle node, Ark_Int32 flag) {}
Ark_Int32 GetCustomMethodFlag(Ark_NodeHandle node) {
    return 0;
}

void SetCustomCallback(Ark_VMContext context, Ark_NodeHandle node, Ark_Int32 callback) {}
void SetCustomNodeDestroyCallback(void (*destroy)(Ark_NodeHandle nodeId)) {}

Ark_Int32 MeasureNode(Ark_VMContext vmContext, Ark_NodeHandle node, Ark_Float32* data) {
    return AsNode(node)->measure(vmContext, data);
}

Ark_Int32 LayoutNode(Ark_VMContext vmContext, Ark_NodeHandle node, Ark_Float32 (*data)[2]) {
    return AsNode(node)->layout(vmContext, (Ark_Float32*)data);
}

Ark_Int32 DrawNode(Ark_VMContext vmContext, Ark_NodeHandle node, Ark_Float32* data) {
    return AsNode(node)->draw(vmContext, data);
}

Ark_Int32 MeasureLayoutAndDraw(Ark_VMContext vmContext, Ark_NodeHandle root) {
    Ark_Float32 rootMeasures[] = {800, 600, 800, 600};
    MeasureNode(vmContext, root, &rootMeasures[0]);
    Ark_Float32 rootLayouts[] = {0, 0, 800, 600};
    LayoutNode(vmContext, root, reinterpret_cast<Ark_Float32(*)[2]>(&rootLayouts));
    Ark_Float32 rootDraw[] = {0, 0, 800, 600};
    DrawNode(vmContext, root, &rootDraw[0]);
    Ark_Int32 result = 0;
    if (!needGroupedLog(1)) {
        return result;
    }
    string out("measureLayoutAndDraw(");
    WriteToString(&out, root);
    out.append(")");
    appendGroupedLog(1, out);
    return result;
}

void SetAttachNodePtr(Ark_NodeHandle node, void* value) {}
void* GetAttachNodePtr(Ark_NodeHandle node) {
    return nullptr;
}
void SetMeasureWidth(Ark_NodeHandle node, Ark_Int32 value) {}

Ark_Int32 GetMeasureWidth(Ark_NodeHandle node) {
    return 0;
}

void SetMeasureHeight(Ark_NodeHandle node, Ark_Int32 value) {}
Ark_Int32 GetMeasureHeight(Ark_NodeHandle node) {
    return 0;
}
void SetX(Ark_NodeHandle node, Ark_Int32 value) {}
void SetY(Ark_NodeHandle node, Ark_Int32 value) {}
Ark_Int32 GetX(Ark_NodeHandle node) {
    return 0;
}
Ark_Int32 GetY(Ark_NodeHandle node) {
    return 0;
}
void SetAlignment(Ark_NodeHandle node, Ark_Int32 value) {}
Ark_Int32 GetAlignment(Ark_NodeHandle node) {
    return 0;
}
void GetLayoutConstraint(Ark_NodeHandle node, Ark_Int32* value) {}
Ark_Int32 IndexerChecker(Ark_VMContext vmContext, Ark_NodeHandle nodePtr) {
    return 0;
}
void SetRangeUpdater(Ark_NodeHandle nodePtr, Ark_Int32 updaterId) {}
void SetLazyItemIndexer(Ark_VMContext vmContext, Ark_NodeHandle nodePtr, Ark_Int32 indexerId) {}
Ark_PipelineContext GetPipelineContext(Ark_NodeHandle node) {
    return (Ark_PipelineContext)42;
}
void SetVsyncCallback(Ark_PipelineContext pipelineContext, Ark_VsyncCallback callback) {
    using namespace std::chrono_literals;
    auto producer = std::thread([pipelineContext, callback] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            callback(pipelineContext);
        }
    });
    producer.detach();
}
void SetChildTotalCount(Ark_NodeHandle node, Ark_Int32 totalCount) {}
void ShowCrash(Ark_CharPtr message) {}
}

Ark_NativePointer NeedMoreElements(Ark_NativePointer node,
                                   Ark_NativePointer mark,
                                   Ark_Int32 direction) {
    return AsNode(node)->needMoreElements(mark, direction);
}

struct NodeData {
    Callback_RangeUpdate updater;
    NodeData(const Callback_RangeUpdate* updater): updater(*updater) {}
};
void OnRangeUpdate(Ark_NativePointer nodePtr,
                   Ark_Int32 totalCount,
                  const Callback_RangeUpdate* updater) {
    auto* node = AsNode(nodePtr);
    updater->resource.hold(updater->resource.resourceId);
    node->setCustomVoidData(new NodeData(updater));
}

void SetCurrentIndex(Ark_NativePointer nodePtr,
                     Ark_Int32 index) {
    auto* node = AsNode(nodePtr);
    Ark_NativePointer mark = (Ark_NativePointer)0x1;
    if (index >= 0 && index < (int)node->children()->size()) {
        mark = (*node->children())[index];
    }
    NodeData* data = (NodeData*)node->customVoidData();
    if (data) {
        data->updater.call(data->updater.resource.resourceId, index, mark, 1000);
    }
}


namespace GeneratedEvents {
    const GENERATED_ArkUIEventsAPI* g_OverriddenEventsImpl = nullptr;
    const GENERATED_ArkUIEventsAPI* GENERATED_GetArkUiEventsAPI() { return g_OverriddenEventsImpl; }
    void GENERATED_SetArkUiEventsAPI(const GENERATED_ArkUIEventsAPI* api) { g_OverriddenEventsImpl = api; }
}
}

// handWritten implementations
namespace OHOS::Ace::NG::GeneratedModifier {
    namespace LazyForEachOpsAccessor {
    void DestroyPeerImpl(LazyForEachOpsPeer* peer)
    {
        if (!needGroupedLog(1))
            return;
        string out("destroyPeer(");
        out.append(") \n");
        appendGroupedLog(1, out);
    }
    Ark_NativePointer CtorImpl()
    {
        if (!needGroupedLog(1))
            return (LazyForEachOpsPeer*) 100;
        string out("new LazyForEachOps(");
        out.append(") \n");
        out.append("[return (LazyForEachOpsPeer*) 100] \n");
        appendGroupedLog(1, out);
        return (LazyForEachOpsPeer*) 100;
    }
    Ark_NativePointer GetFinalizerImpl()
    {
        if (!needGroupedLog(1))
            return fnPtr<KNativePointer>(dummyClassFinalizer);
        string out("getFinalizer(");
        out.append(") \n");
        out.append("[return fnPtr<KNativePointer>(dummyClassFinalizer)] \n");
        appendGroupedLog(1, out);
        return fnPtr<KNativePointer>(dummyClassFinalizer);
    }
    Ark_NativePointer NeedMoreElementsImpl(Ark_NativePointer node,
                                           Ark_NativePointer mark,
                                           Ark_Int32 direction)
    {
        if (!needGroupedLog(1))
            return (void*) 300;
        string out("NeedMoreElements(");
        WriteToString(&out, node);
        out.append(", ");
        WriteToString(&out, mark);
        out.append(", ");
        WriteToString(&out, direction);
        out.append(") \n");
        out.append("[return (void*) 300] \n");
        appendGroupedLog(1, out);
        return (void*) 300;
    }
    void OnRangeUpdateImpl(Ark_NativePointer node,
                           Ark_Int32 totalCount,
                           const Callback_RangeUpdate* updater)
    {
        if (!needGroupedLog(1))
            return;
        string out("OnRangeUpdate(");
        WriteToString(&out, node);
        out.append(", ");
        WriteToString(&out, totalCount);
        out.append(", ");
        WriteToString(&out, updater);
        out.append(") \n");
        appendGroupedLog(1, out);
    }
    void SetCurrentIndexImpl(Ark_NativePointer node,
                             Ark_Int32 index)
    {
        if (!needGroupedLog(1))
            return;
        string out("SetCurrentIndex(");
        WriteToString(&out, node);
        out.append(", ");
        WriteToString(&out, index);
        out.append(") \n");
        appendGroupedLog(1, out);
    }
    void PrepareImpl(Ark_NativePointer node)
    {
        if (!needGroupedLog(1))
            return;
        string out("Prepare(");
        WriteToString(&out, node);
        out.append(") \n");
        appendGroupedLog(1, out);
    }
    } // LazyForEachOpsAccessor

    namespace CommonMethodModifier {
        void OnClick0Impl(Ark_NativePointer node,
                      const Callback_ClickEvent_Void* value)
    {
        if (!needGroupedLog(1))
            return;
        string out("onClick(");
        WriteToString(&out, value);
        out.append(") \n");
        appendGroupedLog(1, out);
    }
    void OnClick1Impl(Ark_NativePointer node,
                      const Callback_ClickEvent_Void* event,
                      const Ark_Number* distanceThreshold)
    {
        if (!needGroupedLog(1))
            return;
        string out("onClick(");
        WriteToString(&out, event);
        out.append(", ");
        WriteToString(&out, distanceThreshold);
        out.append(") \n");
        appendGroupedLog(1, out);
    }
    } // CommonMethodModifier
}

// end of handWritten implementations
namespace OHOS::Ace::NG::GeneratedModifier {
    const GENERATED_ArkUINodeModifiers* GENERATED_GetArkUINodeModifiers()
    {
        static const GENERATED_ArkUINodeModifiers modifiersImpl = {
        };
        return &modifiersImpl;
    }
    namespace AudioManagerAccessor {
    void DestroyPeerImpl(AudioManagerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioManagerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioManagerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void SetVolume0Impl(AudioManagerPeer* peer,
                        OH_AUDIO_audio_AudioVolumeType volumeType,
                        const OH_Number* volume,
                        const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetVolume1Impl(AudioManagerPeer* peer,
                        OH_AUDIO_audio_AudioVolumeType volumeType,
                        const OH_Number* volume)
    {
    }
    void GetVolume0Impl(AudioManagerPeer* peer,
                        OH_AUDIO_audio_AudioVolumeType volumeType,
                        const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetVolume1Impl(AudioManagerPeer* peer,
                            OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void GetMinVolume0Impl(AudioManagerPeer* peer,
                           OH_AUDIO_audio_AudioVolumeType volumeType,
                           const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetMinVolume1Impl(AudioManagerPeer* peer,
                               OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void GetMaxVolume0Impl(AudioManagerPeer* peer,
                           OH_AUDIO_audio_AudioVolumeType volumeType,
                           const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetMaxVolume1Impl(AudioManagerPeer* peer,
                               OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void GetDevices0Impl(AudioManagerPeer* peer,
                         OH_AUDIO_audio_DeviceFlag deviceFlag,
                         const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    OH_NativePointer GetDevices1Impl(AudioManagerPeer* peer,
                                     OH_AUDIO_audio_DeviceFlag deviceFlag)
    {
        return nullptr;
    }
    void Mute0Impl(AudioManagerPeer* peer,
                   OH_AUDIO_audio_AudioVolumeType volumeType,
                   OH_Boolean mute,
                   const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Mute1Impl(AudioManagerPeer* peer,
                   OH_AUDIO_audio_AudioVolumeType volumeType,
                   OH_Boolean mute)
    {
    }
    void IsMute0Impl(AudioManagerPeer* peer,
                     OH_AUDIO_audio_AudioVolumeType volumeType,
                     const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsMute1Impl(AudioManagerPeer* peer,
                           OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void IsActive0Impl(AudioManagerPeer* peer,
                       OH_AUDIO_audio_AudioVolumeType volumeType,
                       const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsActive1Impl(AudioManagerPeer* peer,
                             OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void SetMicrophoneMute0Impl(AudioManagerPeer* peer,
                                OH_Boolean mute,
                                const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetMicrophoneMute1Impl(AudioManagerPeer* peer,
                                OH_Boolean mute)
    {
    }
    void IsMicrophoneMute0Impl(AudioManagerPeer* peer,
                               const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsMicrophoneMute1Impl(AudioManagerPeer* peer)
    {
        return 0;
    }
    void SetRingerMode0Impl(AudioManagerPeer* peer,
                            OH_AUDIO_audio_AudioRingMode mode,
                            const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetRingerMode1Impl(AudioManagerPeer* peer,
                            OH_AUDIO_audio_AudioRingMode mode)
    {
    }
    void GetRingerMode0Impl(AudioManagerPeer* peer,
                            const AUDIO_AsyncCallback_AudioRingMode_Void* callback_)
    {
    }
    OH_NativePointer GetRingerMode1Impl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    void SetAudioParameter0Impl(AudioManagerPeer* peer,
                                const OH_String* key,
                                const OH_String* value,
                                const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetAudioParameter1Impl(AudioManagerPeer* peer,
                                const OH_String* key,
                                const OH_String* value)
    {
    }
    void GetAudioParameter0Impl(AudioManagerPeer* peer,
                                const OH_String* key,
                                const AUDIO_AsyncCallback_String_Void* callback_)
    {
    }
    void GetAudioParameter1Impl(AudioManagerPeer* peer,
                                const OH_String* key)
    {
    }
    void SetExtraParametersImpl(AudioManagerPeer* peer,
                                const OH_String* mainKey,
                                const Map_String_String* kvpairs)
    {
    }
    OH_NativePointer GetExtraParametersImpl(AudioManagerPeer* peer,
                                            const OH_String* mainKey,
                                            const Opt_Array_String* subKeys)
    {
        return 0;
    }
    void SetDeviceActive0Impl(AudioManagerPeer* peer,
                              OH_AUDIO_audio_ActiveDeviceType deviceType,
                              OH_Boolean active,
                              const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetDeviceActive1Impl(AudioManagerPeer* peer,
                              OH_AUDIO_audio_ActiveDeviceType deviceType,
                              OH_Boolean active)
    {
    }
    void IsDeviceActive0Impl(AudioManagerPeer* peer,
                             OH_AUDIO_audio_ActiveDeviceType deviceType,
                             const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsDeviceActive1Impl(AudioManagerPeer* peer,
                                   OH_AUDIO_audio_ActiveDeviceType deviceType)
    {
        return 0;
    }
    void OnVolumeChangeImpl(AudioManagerPeer* peer,
                            const AUDIO_Callback_VolumeEvent_Void* callback_)
    {
    }
    void OnRingerModeChangeImpl(AudioManagerPeer* peer,
                                const AUDIO_Callback_AudioRingMode_Void* callback_)
    {
    }
    void SetAudioScene0Impl(AudioManagerPeer* peer,
                            OH_AUDIO_audio_AudioScene scene,
                            const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetAudioScene1Impl(AudioManagerPeer* peer,
                            OH_AUDIO_audio_AudioScene scene)
    {
    }
    void GetAudioScene0Impl(AudioManagerPeer* peer,
                            const AUDIO_AsyncCallback_AudioScene_Void* callback_)
    {
    }
    OH_NativePointer GetAudioScene1Impl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetAudioSceneSyncImpl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    void OnDeviceChangeImpl(AudioManagerPeer* peer,
                            const AUDIO_Callback_DeviceChangeAction_Void* callback_)
    {
    }
    void OffDeviceChangeImpl(AudioManagerPeer* peer,
                             const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_)
    {
    }
    void OnInterruptImpl(AudioManagerPeer* peer,
                         const OH_AUDIO_AudioInterrupt* interrupt,
                         const AUDIO_Callback_InterruptAction_Void* callback_)
    {
    }
    void OffInterruptImpl(AudioManagerPeer* peer,
                          const OH_AUDIO_AudioInterrupt* interrupt,
                          const Opt_AUDIO_Callback_InterruptAction_Void* callback_)
    {
    }
    OH_NativePointer GetVolumeManagerImpl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetStreamManagerImpl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetRoutingManagerImpl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetSessionManagerImpl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetSpatializationManagerImpl(AudioManagerPeer* peer)
    {
        return nullptr;
    }
    void DisableSafeMediaVolumeImpl(AudioManagerPeer* peer)
    {
    }
    } // AudioManagerAccessor
    namespace AudioRoutingManagerAccessor {
    void DestroyPeerImpl(AudioRoutingManagerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioRoutingManagerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioRoutingManagerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void GetDevices0Impl(AudioRoutingManagerPeer* peer,
                         OH_AUDIO_audio_DeviceFlag deviceFlag,
                         const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    OH_NativePointer GetDevices1Impl(AudioRoutingManagerPeer* peer,
                                     OH_AUDIO_audio_DeviceFlag deviceFlag)
    {
        return nullptr;
    }
    OH_NativePointer GetDevicesSyncImpl(AudioRoutingManagerPeer* peer,
                                        OH_AUDIO_audio_DeviceFlag deviceFlag)
    {
        return nullptr;
    }
    void OnDeviceChangeImpl(AudioRoutingManagerPeer* peer,
                            OH_AUDIO_audio_DeviceFlag deviceFlag,
                            const AUDIO_Callback_DeviceChangeAction_Void* callback_)
    {
    }
    void OffDeviceChangeImpl(AudioRoutingManagerPeer* peer,
                             const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_)
    {
    }
    OH_NativePointer GetAvailableDevicesImpl(AudioRoutingManagerPeer* peer,
                                             OH_AUDIO_audio_DeviceUsage deviceUsage)
    {
        return nullptr;
    }
    void OnAvailableDeviceChangeImpl(AudioRoutingManagerPeer* peer,
                                     OH_AUDIO_audio_DeviceUsage deviceUsage,
                                     const AUDIO_Callback_DeviceChangeAction_Void* callback_)
    {
    }
    void OffAvailableDeviceChangeImpl(AudioRoutingManagerPeer* peer,
                                      const Opt_AUDIO_Callback_DeviceChangeAction_Void* callback_)
    {
    }
    void SetCommunicationDevice0Impl(AudioRoutingManagerPeer* peer,
                                     OH_AUDIO_audio_CommunicationDeviceType deviceType,
                                     OH_Boolean active,
                                     const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetCommunicationDevice1Impl(AudioRoutingManagerPeer* peer,
                                     OH_AUDIO_audio_CommunicationDeviceType deviceType,
                                     OH_Boolean active)
    {
    }
    void IsCommunicationDeviceActive0Impl(AudioRoutingManagerPeer* peer,
                                          OH_AUDIO_audio_CommunicationDeviceType deviceType,
                                          const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsCommunicationDeviceActive1Impl(AudioRoutingManagerPeer* peer,
                                                OH_AUDIO_audio_CommunicationDeviceType deviceType)
    {
        return 0;
    }
    OH_Boolean IsCommunicationDeviceActiveSyncImpl(AudioRoutingManagerPeer* peer,
                                                   OH_AUDIO_audio_CommunicationDeviceType deviceType)
    {
        return 0;
    }
    void SelectOutputDevice0Impl(AudioRoutingManagerPeer* peer,
                                 const Array_CustomObject* outputAudioDevices,
                                 const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SelectOutputDevice1Impl(AudioRoutingManagerPeer* peer,
                                 const Array_CustomObject* outputAudioDevices)
    {
    }
    void SelectOutputDeviceByFilter0Impl(AudioRoutingManagerPeer* peer,
                                         const OH_AUDIO_AudioRendererFilter* filter,
                                         const Array_CustomObject* outputAudioDevices,
                                         const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SelectOutputDeviceByFilter1Impl(AudioRoutingManagerPeer* peer,
                                         const OH_AUDIO_AudioRendererFilter* filter,
                                         const Array_CustomObject* outputAudioDevices)
    {
    }
    void SelectInputDevice0Impl(AudioRoutingManagerPeer* peer,
                                const Array_CustomObject* inputAudioDevices,
                                const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SelectInputDevice1Impl(AudioRoutingManagerPeer* peer,
                                const Array_CustomObject* inputAudioDevices)
    {
    }
    void SelectInputDeviceByFilterImpl(AudioRoutingManagerPeer* peer,
                                       const OH_AUDIO_AudioCapturerFilter* filter,
                                       const Array_CustomObject* inputAudioDevices)
    {
    }
    void GetPreferOutputDeviceForRendererInfo0Impl(AudioRoutingManagerPeer* peer,
                                                   const OH_AUDIO_AudioRendererInfo* rendererInfo,
                                                   const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    OH_NativePointer GetPreferOutputDeviceForRendererInfo1Impl(AudioRoutingManagerPeer* peer,
                                                               const OH_AUDIO_AudioRendererInfo* rendererInfo)
    {
        return nullptr;
    }
    OH_NativePointer GetPreferredOutputDeviceForRendererInfoSyncImpl(AudioRoutingManagerPeer* peer,
                                                                     const OH_AUDIO_AudioRendererInfo* rendererInfo)
    {
        return nullptr;
    }
    OH_NativePointer GetPreferredOutputDeviceByFilterImpl(AudioRoutingManagerPeer* peer,
                                                          const OH_AUDIO_AudioRendererFilter* filter)
    {
        return nullptr;
    }
    void OnPreferOutputDeviceChangeForRendererInfoImpl(AudioRoutingManagerPeer* peer,
                                                       const OH_AUDIO_AudioRendererInfo* rendererInfo,
                                                       const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    void OffPreferOutputDeviceChangeForRendererInfoImpl(AudioRoutingManagerPeer* peer,
                                                        const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    void GetPreferredInputDeviceForCapturerInfo0Impl(AudioRoutingManagerPeer* peer,
                                                     const OH_AUDIO_AudioCapturerInfo* capturerInfo,
                                                     const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    OH_NativePointer GetPreferredInputDeviceForCapturerInfo1Impl(AudioRoutingManagerPeer* peer,
                                                                 const OH_AUDIO_AudioCapturerInfo* capturerInfo)
    {
        return nullptr;
    }
    OH_NativePointer GetPreferredInputDeviceByFilterImpl(AudioRoutingManagerPeer* peer,
                                                         const OH_AUDIO_AudioCapturerFilter* filter)
    {
        return nullptr;
    }
    void OnPreferredInputDeviceChangeForCapturerInfoImpl(AudioRoutingManagerPeer* peer,
                                                         const OH_AUDIO_AudioCapturerInfo* capturerInfo,
                                                         const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    void OffPreferredInputDeviceChangeForCapturerInfoImpl(AudioRoutingManagerPeer* peer,
                                                          const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    OH_NativePointer GetPreferredInputDeviceForCapturerInfoSyncImpl(AudioRoutingManagerPeer* peer,
                                                                    const OH_AUDIO_AudioCapturerInfo* capturerInfo)
    {
        return nullptr;
    }
    OH_Boolean IsMicBlockDetectionSupportedImpl(AudioRoutingManagerPeer* peer)
    {
        return 0;
    }
    void OnMicBlockStatusChangedImpl(AudioRoutingManagerPeer* peer,
                                     const AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_)
    {
    }
    void OffMicBlockStatusChangedImpl(AudioRoutingManagerPeer* peer,
                                      const Opt_AUDIO_Callback_DeviceBlockStatusInfo_Void* callback_)
    {
    }
    } // AudioRoutingManagerAccessor
    namespace AudioStreamManagerAccessor {
    void DestroyPeerImpl(AudioStreamManagerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioStreamManagerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioStreamManagerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void GetCurrentAudioRendererInfoArray0Impl(AudioStreamManagerPeer* peer,
                                               const AUDIO_AsyncCallback_AudioRendererChangeInfoArray_Void* callback_)
    {
    }
    OH_NativePointer GetCurrentAudioRendererInfoArray1Impl(AudioStreamManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetCurrentAudioRendererInfoArraySyncImpl(AudioStreamManagerPeer* peer)
    {
        return nullptr;
    }
    void GetCurrentAudioCapturerInfoArray0Impl(AudioStreamManagerPeer* peer,
                                               const AUDIO_AsyncCallback_AudioCapturerChangeInfoArray_Void* callback_)
    {
    }
    OH_NativePointer GetCurrentAudioCapturerInfoArray1Impl(AudioStreamManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetCurrentAudioCapturerInfoArraySyncImpl(AudioStreamManagerPeer* peer)
    {
        return nullptr;
    }
    void GetAudioEffectInfoArray0Impl(AudioStreamManagerPeer* peer,
                                      OH_AUDIO_audio_StreamUsage usage,
                                      const AUDIO_AsyncCallback_AudioEffectInfoArray_Void* callback_)
    {
    }
    OH_NativePointer GetAudioEffectInfoArray1Impl(AudioStreamManagerPeer* peer,
                                                  OH_AUDIO_audio_StreamUsage usage)
    {
        return nullptr;
    }
    OH_NativePointer GetAudioEffectInfoArraySyncImpl(AudioStreamManagerPeer* peer,
                                                     OH_AUDIO_audio_StreamUsage usage)
    {
        return nullptr;
    }
    void OnAudioRendererChangeImpl(AudioStreamManagerPeer* peer,
                                   const AUDIO_Callback_AudioRendererChangeInfoArray_Void* callback_)
    {
    }
    void OffAudioRendererChangeImpl(AudioStreamManagerPeer* peer)
    {
    }
    void OnAudioCapturerChangeImpl(AudioStreamManagerPeer* peer,
                                   const AUDIO_Callback_AudioCapturerChangeInfoArray_Void* callback_)
    {
    }
    void OffAudioCapturerChangeImpl(AudioStreamManagerPeer* peer)
    {
    }
    void IsActive0Impl(AudioStreamManagerPeer* peer,
                       OH_AUDIO_audio_AudioVolumeType volumeType,
                       const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsActive1Impl(AudioStreamManagerPeer* peer,
                             OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    OH_Boolean IsActiveSyncImpl(AudioStreamManagerPeer* peer,
                                OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    } // AudioStreamManagerAccessor
    namespace AudioSessionManagerAccessor {
    void DestroyPeerImpl(AudioSessionManagerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioSessionManagerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioSessionManagerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void ActivateAudioSessionImpl(AudioSessionManagerPeer* peer,
                                  const OH_AUDIO_AudioSessionStrategy* strategy)
    {
    }
    void DeactivateAudioSessionImpl(AudioSessionManagerPeer* peer)
    {
    }
    OH_Boolean IsAudioSessionActivatedImpl(AudioSessionManagerPeer* peer)
    {
        return 0;
    }
    void OnAudioSessionDeactivatedImpl(AudioSessionManagerPeer* peer,
                                       const AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_)
    {
    }
    void OffAudioSessionDeactivatedImpl(AudioSessionManagerPeer* peer,
                                        const Opt_AUDIO_Callback_AudioSessionDeactivatedEvent_Void* callback_)
    {
    }
    } // AudioSessionManagerAccessor
    namespace AudioVolumeManagerAccessor {
    void DestroyPeerImpl(AudioVolumeManagerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioVolumeManagerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioVolumeManagerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void GetVolumeGroupInfos0Impl(AudioVolumeManagerPeer* peer,
                                  const OH_String* networkId,
                                  const AUDIO_AsyncCallback_VolumeGroupInfos_Void* callback_)
    {
    }
    OH_NativePointer GetVolumeGroupInfos1Impl(AudioVolumeManagerPeer* peer,
                                              const OH_String* networkId)
    {
        return nullptr;
    }
    OH_NativePointer GetVolumeGroupInfosSyncImpl(AudioVolumeManagerPeer* peer,
                                                 const OH_String* networkId)
    {
        return nullptr;
    }
    void GetVolumeGroupManager0Impl(AudioVolumeManagerPeer* peer,
                                    const OH_Number* groupId,
                                    const AUDIO_AsyncCallback_AudioVolumeGroupManager_Void* callback_)
    {
    }
    OH_NativePointer GetVolumeGroupManager1Impl(AudioVolumeManagerPeer* peer,
                                                const OH_Number* groupId)
    {
        return nullptr;
    }
    OH_NativePointer GetVolumeGroupManagerSyncImpl(AudioVolumeManagerPeer* peer,
                                                   const OH_Number* groupId)
    {
        return nullptr;
    }
    void OnVolumeChangeImpl(AudioVolumeManagerPeer* peer,
                            const AUDIO_Callback_VolumeEvent_Void* callback_)
    {
    }
    void OffVolumeChangeImpl(AudioVolumeManagerPeer* peer,
                             const Opt_AUDIO_Callback_VolumeEvent_Void* callback_)
    {
    }
    } // AudioVolumeManagerAccessor
    namespace AudioVolumeGroupManagerAccessor {
    void DestroyPeerImpl(AudioVolumeGroupManagerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioVolumeGroupManagerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioVolumeGroupManagerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void SetVolume0Impl(AudioVolumeGroupManagerPeer* peer,
                        OH_AUDIO_audio_AudioVolumeType volumeType,
                        const OH_Number* volume,
                        const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetVolume1Impl(AudioVolumeGroupManagerPeer* peer,
                        OH_AUDIO_audio_AudioVolumeType volumeType,
                        const OH_Number* volume)
    {
    }
    void SetVolumeWithFlagImpl(AudioVolumeGroupManagerPeer* peer,
                               OH_AUDIO_audio_AudioVolumeType volumeType,
                               const OH_Number* volume,
                               const OH_Number* flags)
    {
    }
    OH_NativePointer GetActiveVolumeTypeSyncImpl(AudioVolumeGroupManagerPeer* peer,
                                                 const OH_Number* uid)
    {
        return nullptr;
    }
    void GetVolume0Impl(AudioVolumeGroupManagerPeer* peer,
                        OH_AUDIO_audio_AudioVolumeType volumeType,
                        const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetVolume1Impl(AudioVolumeGroupManagerPeer* peer,
                            OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    OH_Int32 GetVolumeSyncImpl(AudioVolumeGroupManagerPeer* peer,
                               OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void GetMinVolume0Impl(AudioVolumeGroupManagerPeer* peer,
                           OH_AUDIO_audio_AudioVolumeType volumeType,
                           const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetMinVolume1Impl(AudioVolumeGroupManagerPeer* peer,
                               OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    OH_Int32 GetMinVolumeSyncImpl(AudioVolumeGroupManagerPeer* peer,
                                  OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void GetMaxVolume0Impl(AudioVolumeGroupManagerPeer* peer,
                           OH_AUDIO_audio_AudioVolumeType volumeType,
                           const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetMaxVolume1Impl(AudioVolumeGroupManagerPeer* peer,
                               OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    OH_Int32 GetMaxVolumeSyncImpl(AudioVolumeGroupManagerPeer* peer,
                                  OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void Mute0Impl(AudioVolumeGroupManagerPeer* peer,
                   OH_AUDIO_audio_AudioVolumeType volumeType,
                   OH_Boolean mute,
                   const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Mute1Impl(AudioVolumeGroupManagerPeer* peer,
                   OH_AUDIO_audio_AudioVolumeType volumeType,
                   OH_Boolean mute)
    {
    }
    void IsMute0Impl(AudioVolumeGroupManagerPeer* peer,
                     OH_AUDIO_audio_AudioVolumeType volumeType,
                     const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsMute1Impl(AudioVolumeGroupManagerPeer* peer,
                           OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    OH_Boolean IsMuteSyncImpl(AudioVolumeGroupManagerPeer* peer,
                              OH_AUDIO_audio_AudioVolumeType volumeType)
    {
        return 0;
    }
    void SetRingerMode0Impl(AudioVolumeGroupManagerPeer* peer,
                            OH_AUDIO_audio_AudioRingMode mode,
                            const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetRingerMode1Impl(AudioVolumeGroupManagerPeer* peer,
                            OH_AUDIO_audio_AudioRingMode mode)
    {
    }
    void GetRingerMode0Impl(AudioVolumeGroupManagerPeer* peer,
                            const AUDIO_AsyncCallback_AudioRingMode_Void* callback_)
    {
    }
    OH_NativePointer GetRingerMode1Impl(AudioVolumeGroupManagerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetRingerModeSyncImpl(AudioVolumeGroupManagerPeer* peer)
    {
        return nullptr;
    }
    void OnRingerModeChangeImpl(AudioVolumeGroupManagerPeer* peer,
                                const AUDIO_Callback_AudioRingMode_Void* callback_)
    {
    }
    void SetMicrophoneMute0Impl(AudioVolumeGroupManagerPeer* peer,
                                OH_Boolean mute,
                                const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetMicrophoneMute1Impl(AudioVolumeGroupManagerPeer* peer,
                                OH_Boolean mute)
    {
    }
    void SetMicMuteImpl(AudioVolumeGroupManagerPeer* peer,
                        OH_Boolean mute)
    {
    }
    void SetMicMutePersistentImpl(AudioVolumeGroupManagerPeer* peer,
                                  OH_Boolean mute,
                                  OH_AUDIO_audio_PolicyType type)
    {
    }
    OH_Boolean IsPersistentMicMuteImpl(AudioVolumeGroupManagerPeer* peer)
    {
        return 0;
    }
    void IsMicrophoneMute0Impl(AudioVolumeGroupManagerPeer* peer,
                               const AUDIO_AsyncCallback_Boolean_Void* callback_)
    {
    }
    OH_Boolean IsMicrophoneMute1Impl(AudioVolumeGroupManagerPeer* peer)
    {
        return 0;
    }
    OH_Boolean IsMicrophoneMuteSyncImpl(AudioVolumeGroupManagerPeer* peer)
    {
        return 0;
    }
    void OnMicStateChangeImpl(AudioVolumeGroupManagerPeer* peer,
                              const AUDIO_Callback_MicStateChangeEvent_Void* callback_)
    {
    }
    void OffMicStateChangeImpl(AudioVolumeGroupManagerPeer* peer,
                               const Opt_AUDIO_Callback_MicStateChangeEvent_Void* callback_)
    {
    }
    OH_Boolean IsVolumeUnadjustableImpl(AudioVolumeGroupManagerPeer* peer)
    {
        return 0;
    }
    void AdjustVolumeByStep0Impl(AudioVolumeGroupManagerPeer* peer,
                                 OH_AUDIO_audio_VolumeAdjustType adjustType,
                                 const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void AdjustVolumeByStep1Impl(AudioVolumeGroupManagerPeer* peer,
                                 OH_AUDIO_audio_VolumeAdjustType adjustType)
    {
    }
    void AdjustSystemVolumeByStep0Impl(AudioVolumeGroupManagerPeer* peer,
                                       OH_AUDIO_audio_AudioVolumeType volumeType,
                                       OH_AUDIO_audio_VolumeAdjustType adjustType,
                                       const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void AdjustSystemVolumeByStep1Impl(AudioVolumeGroupManagerPeer* peer,
                                       OH_AUDIO_audio_AudioVolumeType volumeType,
                                       OH_AUDIO_audio_VolumeAdjustType adjustType)
    {
    }
    void GetSystemVolumeInDb0Impl(AudioVolumeGroupManagerPeer* peer,
                                  OH_AUDIO_audio_AudioVolumeType volumeType,
                                  const OH_Number* volumeLevel,
                                  OH_AUDIO_audio_DeviceType device,
                                  const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetSystemVolumeInDb1Impl(AudioVolumeGroupManagerPeer* peer,
                                      OH_AUDIO_audio_AudioVolumeType volumeType,
                                      const OH_Number* volumeLevel,
                                      OH_AUDIO_audio_DeviceType device)
    {
        return 0;
    }
    OH_Int32 GetSystemVolumeInDbSyncImpl(AudioVolumeGroupManagerPeer* peer,
                                         OH_AUDIO_audio_AudioVolumeType volumeType,
                                         const OH_Number* volumeLevel,
                                         OH_AUDIO_audio_DeviceType device)
    {
        return 0;
    }
    OH_Int32 GetMaxAmplitudeForInputDeviceImpl(AudioVolumeGroupManagerPeer* peer,
                                               const OH_AUDIO_AudioDeviceDescriptor* inputDevice)
    {
        return 0;
    }
    OH_Int32 GetMaxAmplitudeForOutputDeviceImpl(AudioVolumeGroupManagerPeer* peer,
                                                const OH_AUDIO_AudioDeviceDescriptor* outputDevice)
    {
        return 0;
    }
    } // AudioVolumeGroupManagerAccessor
    namespace AudioSpatializationManagerAccessor {
    void DestroyPeerImpl(AudioSpatializationManagerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioSpatializationManagerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioSpatializationManagerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    OH_Boolean IsSpatializationSupportedImpl(AudioSpatializationManagerPeer* peer)
    {
        return 0;
    }
    OH_Boolean IsSpatializationSupportedForDeviceImpl(AudioSpatializationManagerPeer* peer,
                                                      const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor)
    {
        return 0;
    }
    OH_Boolean IsHeadTrackingSupportedImpl(AudioSpatializationManagerPeer* peer)
    {
        return 0;
    }
    OH_Boolean IsHeadTrackingSupportedForDeviceImpl(AudioSpatializationManagerPeer* peer,
                                                    const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor)
    {
        return 0;
    }
    void SetSpatializationEnabled0Impl(AudioSpatializationManagerPeer* peer,
                                       OH_Boolean enable,
                                       const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetSpatializationEnabled1Impl(AudioSpatializationManagerPeer* peer,
                                       OH_Boolean enable)
    {
    }
    void SetSpatializationEnabled2Impl(AudioSpatializationManagerPeer* peer,
                                       const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor,
                                       OH_Boolean enabled)
    {
    }
    OH_Boolean IsSpatializationEnabled0Impl(AudioSpatializationManagerPeer* peer)
    {
        return 0;
    }
    OH_Boolean IsSpatializationEnabled1Impl(AudioSpatializationManagerPeer* peer,
                                            const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor)
    {
        return 0;
    }
    void OnSpatializationEnabledChangeImpl(AudioSpatializationManagerPeer* peer,
                                           const AUDIO_Callback_Boolean_Void* callback_)
    {
    }
    void OnSpatializationEnabledChangeForAnyDeviceImpl(AudioSpatializationManagerPeer* peer,
                                                       const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_)
    {
    }
    void OffSpatializationEnabledChangeImpl(AudioSpatializationManagerPeer* peer,
                                            const Opt_AUDIO_Callback_Boolean_Void* callback_)
    {
    }
    void OffSpatializationEnabledChangeForAnyDeviceImpl(AudioSpatializationManagerPeer* peer,
                                                        const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_)
    {
    }
    void SetHeadTrackingEnabled0Impl(AudioSpatializationManagerPeer* peer,
                                     OH_Boolean enable,
                                     const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetHeadTrackingEnabled1Impl(AudioSpatializationManagerPeer* peer,
                                     OH_Boolean enable)
    {
    }
    void SetHeadTrackingEnabled2Impl(AudioSpatializationManagerPeer* peer,
                                     const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor,
                                     OH_Boolean enabled)
    {
    }
    OH_Boolean IsHeadTrackingEnabled0Impl(AudioSpatializationManagerPeer* peer)
    {
        return 0;
    }
    OH_Boolean IsHeadTrackingEnabled1Impl(AudioSpatializationManagerPeer* peer,
                                          const OH_AUDIO_AudioDeviceDescriptor* deviceDescriptor)
    {
        return 0;
    }
    void OnHeadTrackingEnabledChangeImpl(AudioSpatializationManagerPeer* peer,
                                         const AUDIO_Callback_Boolean_Void* callback_)
    {
    }
    void OnHeadTrackingEnabledChangeForAnyDeviceImpl(AudioSpatializationManagerPeer* peer,
                                                     const AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_)
    {
    }
    void OffHeadTrackingEnabledChangeImpl(AudioSpatializationManagerPeer* peer,
                                          const Opt_AUDIO_Callback_Boolean_Void* callback_)
    {
    }
    void OffHeadTrackingEnabledChangeForAnyDeviceImpl(AudioSpatializationManagerPeer* peer,
                                                      const Opt_AUDIO_Callback_AudioSpatialEnabledStateForDevice_Void* callback_)
    {
    }
    void UpdateSpatialDeviceStateImpl(AudioSpatializationManagerPeer* peer,
                                      const OH_AUDIO_AudioSpatialDeviceState* spatialDeviceState)
    {
    }
    void SetSpatializationSceneTypeImpl(AudioSpatializationManagerPeer* peer,
                                        OH_AUDIO_audio_AudioSpatializationSceneType spatializationSceneType)
    {
    }
    OH_NativePointer GetSpatializationSceneTypeImpl(AudioSpatializationManagerPeer* peer)
    {
        return nullptr;
    }
    } // AudioSpatializationManagerAccessor
    namespace AudioRendererAccessor {
    void DestroyPeerImpl(AudioRendererPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioRendererPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioRendererPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void GetRendererInfo0Impl(AudioRendererPeer* peer,
                              const AUDIO_AsyncCallback_AudioRendererInfo_Void* callback_)
    {
    }
    OH_NativePointer GetRendererInfo1Impl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetRendererInfoSyncImpl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    void GetStreamInfo0Impl(AudioRendererPeer* peer,
                            const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_)
    {
    }
    OH_NativePointer GetStreamInfo1Impl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetStreamInfoSyncImpl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    void GetAudioStreamId0Impl(AudioRendererPeer* peer,
                               const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetAudioStreamId1Impl(AudioRendererPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetAudioStreamIdSyncImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void GetAudioEffectMode0Impl(AudioRendererPeer* peer,
                                 const AUDIO_AsyncCallback_AudioEffectMode_Void* callback_)
    {
    }
    OH_NativePointer GetAudioEffectMode1Impl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    void SetAudioEffectMode0Impl(AudioRendererPeer* peer,
                                 OH_AUDIO_audio_AudioEffectMode mode,
                                 const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetAudioEffectMode1Impl(AudioRendererPeer* peer,
                                 OH_AUDIO_audio_AudioEffectMode mode)
    {
    }
    void Start0Impl(AudioRendererPeer* peer,
                    const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Start1Impl(AudioRendererPeer* peer)
    {
    }
    void Write0Impl(AudioRendererPeer* peer,
                    const OH_Buffer* buffer,
                    const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 Write1Impl(AudioRendererPeer* peer,
                        const OH_Buffer* buffer)
    {
        return 0;
    }
    void GetAudioTime0Impl(AudioRendererPeer* peer,
                           const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetAudioTime1Impl(AudioRendererPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetAudioTimeSyncImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void Drain0Impl(AudioRendererPeer* peer,
                    const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Drain1Impl(AudioRendererPeer* peer)
    {
    }
    void FlushImpl(AudioRendererPeer* peer)
    {
    }
    void Pause0Impl(AudioRendererPeer* peer,
                    const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Pause1Impl(AudioRendererPeer* peer)
    {
    }
    void Stop0Impl(AudioRendererPeer* peer,
                   const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Stop1Impl(AudioRendererPeer* peer)
    {
    }
    void Release0Impl(AudioRendererPeer* peer,
                      const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Release1Impl(AudioRendererPeer* peer)
    {
    }
    void GetBufferSize0Impl(AudioRendererPeer* peer,
                            const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetBufferSize1Impl(AudioRendererPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetBufferSizeSyncImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void SetRenderRate0Impl(AudioRendererPeer* peer,
                            OH_AUDIO_audio_AudioRendererRate rate,
                            const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetRenderRate1Impl(AudioRendererPeer* peer,
                            OH_AUDIO_audio_AudioRendererRate rate)
    {
    }
    void SetSpeedImpl(AudioRendererPeer* peer,
                      const OH_Number* speed)
    {
    }
    void GetRenderRate0Impl(AudioRendererPeer* peer,
                            const AUDIO_AsyncCallback_AudioRendererRate_Void* callback_)
    {
    }
    OH_NativePointer GetRenderRate1Impl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetRenderRateSyncImpl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    OH_Int32 GetSpeedImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void SetInterruptMode0Impl(AudioRendererPeer* peer,
                               OH_AUDIO_audio_InterruptMode mode,
                               const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetInterruptMode1Impl(AudioRendererPeer* peer,
                               OH_AUDIO_audio_InterruptMode mode)
    {
    }
    void SetInterruptModeSyncImpl(AudioRendererPeer* peer,
                                  OH_AUDIO_audio_InterruptMode mode)
    {
    }
    void SetVolume0Impl(AudioRendererPeer* peer,
                        const OH_Number* volume,
                        const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void SetVolume1Impl(AudioRendererPeer* peer,
                        const OH_Number* volume)
    {
    }
    OH_Int32 GetVolumeImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void SetVolumeWithRampImpl(AudioRendererPeer* peer,
                               const OH_Number* volume,
                               const OH_Number* duration)
    {
    }
    void GetMinStreamVolume0Impl(AudioRendererPeer* peer,
                                 const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetMinStreamVolume1Impl(AudioRendererPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetMinStreamVolumeSyncImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void GetMaxStreamVolume0Impl(AudioRendererPeer* peer,
                                 const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetMaxStreamVolume1Impl(AudioRendererPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetMaxStreamVolumeSyncImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void GetUnderflowCount0Impl(AudioRendererPeer* peer,
                                const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetUnderflowCount1Impl(AudioRendererPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetUnderflowCountSyncImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void GetCurrentOutputDevices0Impl(AudioRendererPeer* peer,
                                      const AUDIO_AsyncCallback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    OH_NativePointer GetCurrentOutputDevices1Impl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetCurrentOutputDevicesSyncImpl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    void SetChannelBlendModeImpl(AudioRendererPeer* peer,
                                 OH_AUDIO_audio_ChannelBlendMode mode)
    {
    }
    void SetSilentModeAndMixWithOthersImpl(AudioRendererPeer* peer,
                                           OH_Boolean on)
    {
    }
    OH_Boolean GetSilentModeAndMixWithOthersImpl(AudioRendererPeer* peer)
    {
        return 0;
    }
    void SetDefaultOutputDeviceImpl(AudioRendererPeer* peer,
                                    OH_AUDIO_audio_DeviceType deviceType)
    {
    }
    void OnAudioInterruptImpl(AudioRendererPeer* peer,
                              const AUDIO_Callback_InterruptEvent_Void* callback_)
    {
    }
    void OnMarkReachImpl(AudioRendererPeer* peer,
                         const OH_Number* frame,
                         const AUDIO_Callback_Number_Void* callback_)
    {
    }
    void OffMarkReachImpl(AudioRendererPeer* peer)
    {
    }
    void OnPeriodReachImpl(AudioRendererPeer* peer,
                           const OH_Number* frame,
                           const AUDIO_Callback_Number_Void* callback_)
    {
    }
    void OffPeriodReachImpl(AudioRendererPeer* peer)
    {
    }
    void OnStateChangeImpl(AudioRendererPeer* peer,
                           const AUDIO_Callback_AudioState_Void* callback_)
    {
    }
    void OnOutputDeviceChangeImpl(AudioRendererPeer* peer,
                                  const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    void OnOutputDeviceChangeWithInfoImpl(AudioRendererPeer* peer,
                                          const AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_)
    {
    }
    void OffOutputDeviceChangeImpl(AudioRendererPeer* peer,
                                   const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    void OffOutputDeviceChangeWithInfoImpl(AudioRendererPeer* peer,
                                           const Opt_AUDIO_Callback_AudioStreamDeviceChangeInfo_Void* callback_)
    {
    }
    void OnWriteDataImpl(AudioRendererPeer* peer,
                         const AUDIO_AudioRendererWriteDataCallback* callback_)
    {
    }
    void OffWriteDataImpl(AudioRendererPeer* peer,
                          const Opt_AUDIO_AudioRendererWriteDataCallback* callback_)
    {
    }
    OH_NativePointer GetStateImpl(AudioRendererPeer* peer)
    {
        return nullptr;
    }
    } // AudioRendererAccessor
    namespace AudioCapturerAccessor {
    void DestroyPeerImpl(AudioCapturerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AudioCapturerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AudioCapturerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void GetCapturerInfo0Impl(AudioCapturerPeer* peer,
                              const AUDIO_AsyncCallback_AudioCapturerInfo_Void* callback_)
    {
    }
    OH_NativePointer GetCapturerInfo1Impl(AudioCapturerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetCapturerInfoSyncImpl(AudioCapturerPeer* peer)
    {
        return nullptr;
    }
    void GetStreamInfo0Impl(AudioCapturerPeer* peer,
                            const AUDIO_AsyncCallback_AudioStreamInfo_Void* callback_)
    {
    }
    OH_NativePointer GetStreamInfo1Impl(AudioCapturerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetStreamInfoSyncImpl(AudioCapturerPeer* peer)
    {
        return nullptr;
    }
    void GetAudioStreamId0Impl(AudioCapturerPeer* peer,
                               const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetAudioStreamId1Impl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetAudioStreamIdSyncImpl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    void Start0Impl(AudioCapturerPeer* peer,
                    const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Start1Impl(AudioCapturerPeer* peer)
    {
    }
    void Read0Impl(AudioCapturerPeer* peer,
                   const OH_Number* size,
                   OH_Boolean isBlockingRead,
                   const AUDIO_AsyncCallback_Buffer_Void* callback_)
    {
    }
    void Read1Impl(AudioCapturerPeer* peer,
                   const OH_Number* size,
                   OH_Boolean isBlockingRead)
    {
    }
    void GetAudioTime0Impl(AudioCapturerPeer* peer,
                           const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetAudioTime1Impl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetAudioTimeSyncImpl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    void Stop0Impl(AudioCapturerPeer* peer,
                   const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Stop1Impl(AudioCapturerPeer* peer)
    {
    }
    void Release0Impl(AudioCapturerPeer* peer,
                      const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Release1Impl(AudioCapturerPeer* peer)
    {
    }
    void GetBufferSize0Impl(AudioCapturerPeer* peer,
                            const AUDIO_AsyncCallback_Number_Void* callback_)
    {
    }
    OH_Int32 GetBufferSize1Impl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetBufferSizeSyncImpl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    OH_NativePointer GetCurrentInputDevicesImpl(AudioCapturerPeer* peer)
    {
        return nullptr;
    }
    OH_NativePointer GetCurrentAudioCapturerChangeInfoImpl(AudioCapturerPeer* peer)
    {
        return nullptr;
    }
    OH_Int32 GetOverflowCountImpl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    OH_Int32 GetOverflowCountSyncImpl(AudioCapturerPeer* peer)
    {
        return 0;
    }
    void OnMarkReachImpl(AudioCapturerPeer* peer,
                         const OH_Number* frame,
                         const AUDIO_Callback_Number_Void* callback_)
    {
    }
    void OffMarkReachImpl(AudioCapturerPeer* peer)
    {
    }
    void OnPeriodReachImpl(AudioCapturerPeer* peer,
                           const OH_Number* frame,
                           const AUDIO_Callback_Number_Void* callback_)
    {
    }
    void OffPeriodReachImpl(AudioCapturerPeer* peer)
    {
    }
    void OnStateChangeImpl(AudioCapturerPeer* peer,
                           const AUDIO_Callback_AudioState_Void* callback_)
    {
    }
    void OnAudioInterruptImpl(AudioCapturerPeer* peer,
                              const AUDIO_Callback_InterruptEvent_Void* callback_)
    {
    }
    void OffAudioInterruptImpl(AudioCapturerPeer* peer)
    {
    }
    void OnInputDeviceChangeImpl(AudioCapturerPeer* peer,
                                 const AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    void OffInputDeviceChangeImpl(AudioCapturerPeer* peer,
                                  const Opt_AUDIO_Callback_AudioDeviceDescriptors_Void* callback_)
    {
    }
    void OnAudioCapturerChangeImpl(AudioCapturerPeer* peer,
                                   const AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_)
    {
    }
    void OffAudioCapturerChangeImpl(AudioCapturerPeer* peer,
                                    const Opt_AUDIO_Callback_AudioCapturerChangeInfo_Void* callback_)
    {
    }
    void OnReadDataImpl(AudioCapturerPeer* peer,
                        const AUDIO_Callback_Buffer_Void* callback_)
    {
    }
    void OffReadDataImpl(AudioCapturerPeer* peer,
                         const Opt_AUDIO_Callback_Buffer_Void* callback_)
    {
    }
    OH_NativePointer GetStateImpl(AudioCapturerPeer* peer)
    {
        return nullptr;
    }
    } // AudioCapturerAccessor
    namespace AsrProcessingControllerAccessor {
    void DestroyPeerImpl(AsrProcessingControllerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<AsrProcessingControllerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new AsrProcessingControllerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    OH_Boolean SetAsrAecModeImpl(AsrProcessingControllerPeer* peer,
                                 OH_AUDIO_audio_AsrAecMode mode)
    {
        return 0;
    }
    OH_NativePointer GetAsrAecModeImpl(AsrProcessingControllerPeer* peer)
    {
        return nullptr;
    }
    OH_Boolean SetAsrNoiseSuppressionModeImpl(AsrProcessingControllerPeer* peer,
                                              OH_AUDIO_audio_AsrNoiseSuppressionMode mode)
    {
        return 0;
    }
    OH_NativePointer GetAsrNoiseSuppressionModeImpl(AsrProcessingControllerPeer* peer)
    {
        return nullptr;
    }
    OH_Boolean IsWhisperingImpl(AsrProcessingControllerPeer* peer)
    {
        return 0;
    }
    OH_Boolean SetAsrVoiceControlModeImpl(AsrProcessingControllerPeer* peer,
                                          OH_AUDIO_audio_AsrVoiceControlMode mode,
                                          OH_Boolean enable)
    {
        return 0;
    }
    OH_Boolean SetAsrVoiceMuteModeImpl(AsrProcessingControllerPeer* peer,
                                       OH_AUDIO_audio_AsrVoiceMuteMode mode,
                                       OH_Boolean enable)
    {
        return 0;
    }
    OH_Boolean SetAsrWhisperDetectionModeImpl(AsrProcessingControllerPeer* peer,
                                              OH_AUDIO_audio_AsrWhisperDetectionMode mode)
    {
        return 0;
    }
    OH_NativePointer GetAsrWhisperDetectionModeImpl(AsrProcessingControllerPeer* peer)
    {
        return nullptr;
    }
    } // AsrProcessingControllerAccessor
    namespace TonePlayerAccessor {
    void DestroyPeerImpl(TonePlayerPeer* peer)
    {
        auto peerImpl = reinterpret_cast<TonePlayerPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer CtorImpl()
    {
        return new TonePlayerPeer();
    }
    OH_NativePointer GetFinalizerImpl()
    {
        return reinterpret_cast<void *>(&DestroyPeerImpl);
    }
    void Load0Impl(TonePlayerPeer* peer,
                   OH_AUDIO_audio_ToneType type,
                   const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Load1Impl(TonePlayerPeer* peer,
                   OH_AUDIO_audio_ToneType type)
    {
    }
    void Start0Impl(TonePlayerPeer* peer,
                    const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Start1Impl(TonePlayerPeer* peer)
    {
    }
    void Stop0Impl(TonePlayerPeer* peer,
                   const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Stop1Impl(TonePlayerPeer* peer)
    {
    }
    void Release0Impl(TonePlayerPeer* peer,
                      const AUDIO_AsyncCallback_Void* callback_)
    {
    }
    void Release1Impl(TonePlayerPeer* peer)
    {
    }
    } // TonePlayerAccessor
    namespace GlobalScope_ohos_multimedia_audioAccessor {
    void DestroyPeerImpl(GlobalScope_ohos_multimedia_audioPeer* peer)
    {
        auto peerImpl = reinterpret_cast<GlobalScope_ohos_multimedia_audioPeerImpl *>(peer);
        if (peerImpl) {
            delete peerImpl;
        }
    }
    OH_NativePointer GetAudioManagerImpl()
    {
        return nullptr;
    }
    void CreateAudioCapturer0Impl(const OH_CustomObject* options,
                                  const OH_CustomObject* callback_)
    {
    }
    OH_NativePointer CreateAudioCapturer1Impl(const OH_CustomObject* options)
    {
        return nullptr;
    }
    void CreateAudioRenderer0Impl(const OH_CustomObject* options,
                                  const OH_CustomObject* callback_)
    {
    }
    OH_NativePointer CreateAudioRenderer1Impl(const OH_CustomObject* options)
    {
        return nullptr;
    }
    void CreateTonePlayer0Impl(const OH_CustomObject* options,
                               const OH_CustomObject* callback_)
    {
    }
    OH_NativePointer CreateTonePlayer1Impl(const OH_CustomObject* options)
    {
        return nullptr;
    }
    OH_NativePointer CreateAsrProcessingControllerImpl(const OH_CustomObject* audioCapturer)
    {
        return nullptr;
    }
    } // GlobalScope_ohos_multimedia_audioAccessor
    const GENERATED_ArkUIAudioManagerAccessor* GetAudioManagerAccessor()
    {
        static const GENERATED_ArkUIAudioManagerAccessor AudioManagerAccessorImpl {
            AudioManagerAccessor::DestroyPeerImpl,
            AudioManagerAccessor::CtorImpl,
            AudioManagerAccessor::GetFinalizerImpl,
            AudioManagerAccessor::SetVolume0Impl,
            AudioManagerAccessor::SetVolume1Impl,
            AudioManagerAccessor::GetVolume0Impl,
            AudioManagerAccessor::GetVolume1Impl,
            AudioManagerAccessor::GetMinVolume0Impl,
            AudioManagerAccessor::GetMinVolume1Impl,
            AudioManagerAccessor::GetMaxVolume0Impl,
            AudioManagerAccessor::GetMaxVolume1Impl,
            AudioManagerAccessor::GetDevices0Impl,
            AudioManagerAccessor::GetDevices1Impl,
            AudioManagerAccessor::Mute0Impl,
            AudioManagerAccessor::Mute1Impl,
            AudioManagerAccessor::IsMute0Impl,
            AudioManagerAccessor::IsMute1Impl,
            AudioManagerAccessor::IsActive0Impl,
            AudioManagerAccessor::IsActive1Impl,
            AudioManagerAccessor::SetMicrophoneMute0Impl,
            AudioManagerAccessor::SetMicrophoneMute1Impl,
            AudioManagerAccessor::IsMicrophoneMute0Impl,
            AudioManagerAccessor::IsMicrophoneMute1Impl,
            AudioManagerAccessor::SetRingerMode0Impl,
            AudioManagerAccessor::SetRingerMode1Impl,
            AudioManagerAccessor::GetRingerMode0Impl,
            AudioManagerAccessor::GetRingerMode1Impl,
            AudioManagerAccessor::SetAudioParameter0Impl,
            AudioManagerAccessor::SetAudioParameter1Impl,
            AudioManagerAccessor::GetAudioParameter0Impl,
            AudioManagerAccessor::GetAudioParameter1Impl,
            AudioManagerAccessor::SetExtraParametersImpl,
            AudioManagerAccessor::GetExtraParametersImpl,
            AudioManagerAccessor::SetDeviceActive0Impl,
            AudioManagerAccessor::SetDeviceActive1Impl,
            AudioManagerAccessor::IsDeviceActive0Impl,
            AudioManagerAccessor::IsDeviceActive1Impl,
            AudioManagerAccessor::OnVolumeChangeImpl,
            AudioManagerAccessor::OnRingerModeChangeImpl,
            AudioManagerAccessor::SetAudioScene0Impl,
            AudioManagerAccessor::SetAudioScene1Impl,
            AudioManagerAccessor::GetAudioScene0Impl,
            AudioManagerAccessor::GetAudioScene1Impl,
            AudioManagerAccessor::GetAudioSceneSyncImpl,
            AudioManagerAccessor::OnDeviceChangeImpl,
            AudioManagerAccessor::OffDeviceChangeImpl,
            AudioManagerAccessor::OnInterruptImpl,
            AudioManagerAccessor::OffInterruptImpl,
            AudioManagerAccessor::GetVolumeManagerImpl,
            AudioManagerAccessor::GetStreamManagerImpl,
            AudioManagerAccessor::GetRoutingManagerImpl,
            AudioManagerAccessor::GetSessionManagerImpl,
            AudioManagerAccessor::GetSpatializationManagerImpl,
            AudioManagerAccessor::DisableSafeMediaVolumeImpl,
        };
        return &AudioManagerAccessorImpl;
    }

    struct AudioManagerPeer {
        virtual ~AudioManagerPeer() = default;
    };
    const GENERATED_ArkUIAudioRoutingManagerAccessor* GetAudioRoutingManagerAccessor()
    {
        static const GENERATED_ArkUIAudioRoutingManagerAccessor AudioRoutingManagerAccessorImpl {
            AudioRoutingManagerAccessor::DestroyPeerImpl,
            AudioRoutingManagerAccessor::CtorImpl,
            AudioRoutingManagerAccessor::GetFinalizerImpl,
            AudioRoutingManagerAccessor::GetDevices0Impl,
            AudioRoutingManagerAccessor::GetDevices1Impl,
            AudioRoutingManagerAccessor::GetDevicesSyncImpl,
            AudioRoutingManagerAccessor::OnDeviceChangeImpl,
            AudioRoutingManagerAccessor::OffDeviceChangeImpl,
            AudioRoutingManagerAccessor::GetAvailableDevicesImpl,
            AudioRoutingManagerAccessor::OnAvailableDeviceChangeImpl,
            AudioRoutingManagerAccessor::OffAvailableDeviceChangeImpl,
            AudioRoutingManagerAccessor::SetCommunicationDevice0Impl,
            AudioRoutingManagerAccessor::SetCommunicationDevice1Impl,
            AudioRoutingManagerAccessor::IsCommunicationDeviceActive0Impl,
            AudioRoutingManagerAccessor::IsCommunicationDeviceActive1Impl,
            AudioRoutingManagerAccessor::IsCommunicationDeviceActiveSyncImpl,
            AudioRoutingManagerAccessor::SelectOutputDevice0Impl,
            AudioRoutingManagerAccessor::SelectOutputDevice1Impl,
            AudioRoutingManagerAccessor::SelectOutputDeviceByFilter0Impl,
            AudioRoutingManagerAccessor::SelectOutputDeviceByFilter1Impl,
            AudioRoutingManagerAccessor::SelectInputDevice0Impl,
            AudioRoutingManagerAccessor::SelectInputDevice1Impl,
            AudioRoutingManagerAccessor::SelectInputDeviceByFilterImpl,
            AudioRoutingManagerAccessor::GetPreferOutputDeviceForRendererInfo0Impl,
            AudioRoutingManagerAccessor::GetPreferOutputDeviceForRendererInfo1Impl,
            AudioRoutingManagerAccessor::GetPreferredOutputDeviceForRendererInfoSyncImpl,
            AudioRoutingManagerAccessor::GetPreferredOutputDeviceByFilterImpl,
            AudioRoutingManagerAccessor::OnPreferOutputDeviceChangeForRendererInfoImpl,
            AudioRoutingManagerAccessor::OffPreferOutputDeviceChangeForRendererInfoImpl,
            AudioRoutingManagerAccessor::GetPreferredInputDeviceForCapturerInfo0Impl,
            AudioRoutingManagerAccessor::GetPreferredInputDeviceForCapturerInfo1Impl,
            AudioRoutingManagerAccessor::GetPreferredInputDeviceByFilterImpl,
            AudioRoutingManagerAccessor::OnPreferredInputDeviceChangeForCapturerInfoImpl,
            AudioRoutingManagerAccessor::OffPreferredInputDeviceChangeForCapturerInfoImpl,
            AudioRoutingManagerAccessor::GetPreferredInputDeviceForCapturerInfoSyncImpl,
            AudioRoutingManagerAccessor::IsMicBlockDetectionSupportedImpl,
            AudioRoutingManagerAccessor::OnMicBlockStatusChangedImpl,
            AudioRoutingManagerAccessor::OffMicBlockStatusChangedImpl,
        };
        return &AudioRoutingManagerAccessorImpl;
    }

    struct AudioRoutingManagerPeer {
        virtual ~AudioRoutingManagerPeer() = default;
    };
    const GENERATED_ArkUIAudioStreamManagerAccessor* GetAudioStreamManagerAccessor()
    {
        static const GENERATED_ArkUIAudioStreamManagerAccessor AudioStreamManagerAccessorImpl {
            AudioStreamManagerAccessor::DestroyPeerImpl,
            AudioStreamManagerAccessor::CtorImpl,
            AudioStreamManagerAccessor::GetFinalizerImpl,
            AudioStreamManagerAccessor::GetCurrentAudioRendererInfoArray0Impl,
            AudioStreamManagerAccessor::GetCurrentAudioRendererInfoArray1Impl,
            AudioStreamManagerAccessor::GetCurrentAudioRendererInfoArraySyncImpl,
            AudioStreamManagerAccessor::GetCurrentAudioCapturerInfoArray0Impl,
            AudioStreamManagerAccessor::GetCurrentAudioCapturerInfoArray1Impl,
            AudioStreamManagerAccessor::GetCurrentAudioCapturerInfoArraySyncImpl,
            AudioStreamManagerAccessor::GetAudioEffectInfoArray0Impl,
            AudioStreamManagerAccessor::GetAudioEffectInfoArray1Impl,
            AudioStreamManagerAccessor::GetAudioEffectInfoArraySyncImpl,
            AudioStreamManagerAccessor::OnAudioRendererChangeImpl,
            AudioStreamManagerAccessor::OffAudioRendererChangeImpl,
            AudioStreamManagerAccessor::OnAudioCapturerChangeImpl,
            AudioStreamManagerAccessor::OffAudioCapturerChangeImpl,
            AudioStreamManagerAccessor::IsActive0Impl,
            AudioStreamManagerAccessor::IsActive1Impl,
            AudioStreamManagerAccessor::IsActiveSyncImpl,
        };
        return &AudioStreamManagerAccessorImpl;
    }

    struct AudioStreamManagerPeer {
        virtual ~AudioStreamManagerPeer() = default;
    };
    const GENERATED_ArkUIAudioSessionManagerAccessor* GetAudioSessionManagerAccessor()
    {
        static const GENERATED_ArkUIAudioSessionManagerAccessor AudioSessionManagerAccessorImpl {
            AudioSessionManagerAccessor::DestroyPeerImpl,
            AudioSessionManagerAccessor::CtorImpl,
            AudioSessionManagerAccessor::GetFinalizerImpl,
            AudioSessionManagerAccessor::ActivateAudioSessionImpl,
            AudioSessionManagerAccessor::DeactivateAudioSessionImpl,
            AudioSessionManagerAccessor::IsAudioSessionActivatedImpl,
            AudioSessionManagerAccessor::OnAudioSessionDeactivatedImpl,
            AudioSessionManagerAccessor::OffAudioSessionDeactivatedImpl,
        };
        return &AudioSessionManagerAccessorImpl;
    }

    struct AudioSessionManagerPeer {
        virtual ~AudioSessionManagerPeer() = default;
    };
    const GENERATED_ArkUIAudioVolumeManagerAccessor* GetAudioVolumeManagerAccessor()
    {
        static const GENERATED_ArkUIAudioVolumeManagerAccessor AudioVolumeManagerAccessorImpl {
            AudioVolumeManagerAccessor::DestroyPeerImpl,
            AudioVolumeManagerAccessor::CtorImpl,
            AudioVolumeManagerAccessor::GetFinalizerImpl,
            AudioVolumeManagerAccessor::GetVolumeGroupInfos0Impl,
            AudioVolumeManagerAccessor::GetVolumeGroupInfos1Impl,
            AudioVolumeManagerAccessor::GetVolumeGroupInfosSyncImpl,
            AudioVolumeManagerAccessor::GetVolumeGroupManager0Impl,
            AudioVolumeManagerAccessor::GetVolumeGroupManager1Impl,
            AudioVolumeManagerAccessor::GetVolumeGroupManagerSyncImpl,
            AudioVolumeManagerAccessor::OnVolumeChangeImpl,
            AudioVolumeManagerAccessor::OffVolumeChangeImpl,
        };
        return &AudioVolumeManagerAccessorImpl;
    }

    struct AudioVolumeManagerPeer {
        virtual ~AudioVolumeManagerPeer() = default;
    };
    const GENERATED_ArkUIAudioVolumeGroupManagerAccessor* GetAudioVolumeGroupManagerAccessor()
    {
        static const GENERATED_ArkUIAudioVolumeGroupManagerAccessor AudioVolumeGroupManagerAccessorImpl {
            AudioVolumeGroupManagerAccessor::DestroyPeerImpl,
            AudioVolumeGroupManagerAccessor::CtorImpl,
            AudioVolumeGroupManagerAccessor::GetFinalizerImpl,
            AudioVolumeGroupManagerAccessor::SetVolume0Impl,
            AudioVolumeGroupManagerAccessor::SetVolume1Impl,
            AudioVolumeGroupManagerAccessor::SetVolumeWithFlagImpl,
            AudioVolumeGroupManagerAccessor::GetActiveVolumeTypeSyncImpl,
            AudioVolumeGroupManagerAccessor::GetVolume0Impl,
            AudioVolumeGroupManagerAccessor::GetVolume1Impl,
            AudioVolumeGroupManagerAccessor::GetVolumeSyncImpl,
            AudioVolumeGroupManagerAccessor::GetMinVolume0Impl,
            AudioVolumeGroupManagerAccessor::GetMinVolume1Impl,
            AudioVolumeGroupManagerAccessor::GetMinVolumeSyncImpl,
            AudioVolumeGroupManagerAccessor::GetMaxVolume0Impl,
            AudioVolumeGroupManagerAccessor::GetMaxVolume1Impl,
            AudioVolumeGroupManagerAccessor::GetMaxVolumeSyncImpl,
            AudioVolumeGroupManagerAccessor::Mute0Impl,
            AudioVolumeGroupManagerAccessor::Mute1Impl,
            AudioVolumeGroupManagerAccessor::IsMute0Impl,
            AudioVolumeGroupManagerAccessor::IsMute1Impl,
            AudioVolumeGroupManagerAccessor::IsMuteSyncImpl,
            AudioVolumeGroupManagerAccessor::SetRingerMode0Impl,
            AudioVolumeGroupManagerAccessor::SetRingerMode1Impl,
            AudioVolumeGroupManagerAccessor::GetRingerMode0Impl,
            AudioVolumeGroupManagerAccessor::GetRingerMode1Impl,
            AudioVolumeGroupManagerAccessor::GetRingerModeSyncImpl,
            AudioVolumeGroupManagerAccessor::OnRingerModeChangeImpl,
            AudioVolumeGroupManagerAccessor::SetMicrophoneMute0Impl,
            AudioVolumeGroupManagerAccessor::SetMicrophoneMute1Impl,
            AudioVolumeGroupManagerAccessor::SetMicMuteImpl,
            AudioVolumeGroupManagerAccessor::SetMicMutePersistentImpl,
            AudioVolumeGroupManagerAccessor::IsPersistentMicMuteImpl,
            AudioVolumeGroupManagerAccessor::IsMicrophoneMute0Impl,
            AudioVolumeGroupManagerAccessor::IsMicrophoneMute1Impl,
            AudioVolumeGroupManagerAccessor::IsMicrophoneMuteSyncImpl,
            AudioVolumeGroupManagerAccessor::OnMicStateChangeImpl,
            AudioVolumeGroupManagerAccessor::OffMicStateChangeImpl,
            AudioVolumeGroupManagerAccessor::IsVolumeUnadjustableImpl,
            AudioVolumeGroupManagerAccessor::AdjustVolumeByStep0Impl,
            AudioVolumeGroupManagerAccessor::AdjustVolumeByStep1Impl,
            AudioVolumeGroupManagerAccessor::AdjustSystemVolumeByStep0Impl,
            AudioVolumeGroupManagerAccessor::AdjustSystemVolumeByStep1Impl,
            AudioVolumeGroupManagerAccessor::GetSystemVolumeInDb0Impl,
            AudioVolumeGroupManagerAccessor::GetSystemVolumeInDb1Impl,
            AudioVolumeGroupManagerAccessor::GetSystemVolumeInDbSyncImpl,
            AudioVolumeGroupManagerAccessor::GetMaxAmplitudeForInputDeviceImpl,
            AudioVolumeGroupManagerAccessor::GetMaxAmplitudeForOutputDeviceImpl,
        };
        return &AudioVolumeGroupManagerAccessorImpl;
    }

    struct AudioVolumeGroupManagerPeer {
        virtual ~AudioVolumeGroupManagerPeer() = default;
    };
    const GENERATED_ArkUIAudioSpatializationManagerAccessor* GetAudioSpatializationManagerAccessor()
    {
        static const GENERATED_ArkUIAudioSpatializationManagerAccessor AudioSpatializationManagerAccessorImpl {
            AudioSpatializationManagerAccessor::DestroyPeerImpl,
            AudioSpatializationManagerAccessor::CtorImpl,
            AudioSpatializationManagerAccessor::GetFinalizerImpl,
            AudioSpatializationManagerAccessor::IsSpatializationSupportedImpl,
            AudioSpatializationManagerAccessor::IsSpatializationSupportedForDeviceImpl,
            AudioSpatializationManagerAccessor::IsHeadTrackingSupportedImpl,
            AudioSpatializationManagerAccessor::IsHeadTrackingSupportedForDeviceImpl,
            AudioSpatializationManagerAccessor::SetSpatializationEnabled0Impl,
            AudioSpatializationManagerAccessor::SetSpatializationEnabled1Impl,
            AudioSpatializationManagerAccessor::SetSpatializationEnabled2Impl,
            AudioSpatializationManagerAccessor::IsSpatializationEnabled0Impl,
            AudioSpatializationManagerAccessor::IsSpatializationEnabled1Impl,
            AudioSpatializationManagerAccessor::OnSpatializationEnabledChangeImpl,
            AudioSpatializationManagerAccessor::OnSpatializationEnabledChangeForAnyDeviceImpl,
            AudioSpatializationManagerAccessor::OffSpatializationEnabledChangeImpl,
            AudioSpatializationManagerAccessor::OffSpatializationEnabledChangeForAnyDeviceImpl,
            AudioSpatializationManagerAccessor::SetHeadTrackingEnabled0Impl,
            AudioSpatializationManagerAccessor::SetHeadTrackingEnabled1Impl,
            AudioSpatializationManagerAccessor::SetHeadTrackingEnabled2Impl,
            AudioSpatializationManagerAccessor::IsHeadTrackingEnabled0Impl,
            AudioSpatializationManagerAccessor::IsHeadTrackingEnabled1Impl,
            AudioSpatializationManagerAccessor::OnHeadTrackingEnabledChangeImpl,
            AudioSpatializationManagerAccessor::OnHeadTrackingEnabledChangeForAnyDeviceImpl,
            AudioSpatializationManagerAccessor::OffHeadTrackingEnabledChangeImpl,
            AudioSpatializationManagerAccessor::OffHeadTrackingEnabledChangeForAnyDeviceImpl,
            AudioSpatializationManagerAccessor::UpdateSpatialDeviceStateImpl,
            AudioSpatializationManagerAccessor::SetSpatializationSceneTypeImpl,
            AudioSpatializationManagerAccessor::GetSpatializationSceneTypeImpl,
        };
        return &AudioSpatializationManagerAccessorImpl;
    }

    struct AudioSpatializationManagerPeer {
        virtual ~AudioSpatializationManagerPeer() = default;
    };
    const GENERATED_ArkUIAudioRendererAccessor* GetAudioRendererAccessor()
    {
        static const GENERATED_ArkUIAudioRendererAccessor AudioRendererAccessorImpl {
            AudioRendererAccessor::DestroyPeerImpl,
            AudioRendererAccessor::CtorImpl,
            AudioRendererAccessor::GetFinalizerImpl,
            AudioRendererAccessor::GetRendererInfo0Impl,
            AudioRendererAccessor::GetRendererInfo1Impl,
            AudioRendererAccessor::GetRendererInfoSyncImpl,
            AudioRendererAccessor::GetStreamInfo0Impl,
            AudioRendererAccessor::GetStreamInfo1Impl,
            AudioRendererAccessor::GetStreamInfoSyncImpl,
            AudioRendererAccessor::GetAudioStreamId0Impl,
            AudioRendererAccessor::GetAudioStreamId1Impl,
            AudioRendererAccessor::GetAudioStreamIdSyncImpl,
            AudioRendererAccessor::GetAudioEffectMode0Impl,
            AudioRendererAccessor::GetAudioEffectMode1Impl,
            AudioRendererAccessor::SetAudioEffectMode0Impl,
            AudioRendererAccessor::SetAudioEffectMode1Impl,
            AudioRendererAccessor::Start0Impl,
            AudioRendererAccessor::Start1Impl,
            AudioRendererAccessor::Write0Impl,
            AudioRendererAccessor::Write1Impl,
            AudioRendererAccessor::GetAudioTime0Impl,
            AudioRendererAccessor::GetAudioTime1Impl,
            AudioRendererAccessor::GetAudioTimeSyncImpl,
            AudioRendererAccessor::Drain0Impl,
            AudioRendererAccessor::Drain1Impl,
            AudioRendererAccessor::FlushImpl,
            AudioRendererAccessor::Pause0Impl,
            AudioRendererAccessor::Pause1Impl,
            AudioRendererAccessor::Stop0Impl,
            AudioRendererAccessor::Stop1Impl,
            AudioRendererAccessor::Release0Impl,
            AudioRendererAccessor::Release1Impl,
            AudioRendererAccessor::GetBufferSize0Impl,
            AudioRendererAccessor::GetBufferSize1Impl,
            AudioRendererAccessor::GetBufferSizeSyncImpl,
            AudioRendererAccessor::SetRenderRate0Impl,
            AudioRendererAccessor::SetRenderRate1Impl,
            AudioRendererAccessor::SetSpeedImpl,
            AudioRendererAccessor::GetRenderRate0Impl,
            AudioRendererAccessor::GetRenderRate1Impl,
            AudioRendererAccessor::GetRenderRateSyncImpl,
            AudioRendererAccessor::GetSpeedImpl,
            AudioRendererAccessor::SetInterruptMode0Impl,
            AudioRendererAccessor::SetInterruptMode1Impl,
            AudioRendererAccessor::SetInterruptModeSyncImpl,
            AudioRendererAccessor::SetVolume0Impl,
            AudioRendererAccessor::SetVolume1Impl,
            AudioRendererAccessor::GetVolumeImpl,
            AudioRendererAccessor::SetVolumeWithRampImpl,
            AudioRendererAccessor::GetMinStreamVolume0Impl,
            AudioRendererAccessor::GetMinStreamVolume1Impl,
            AudioRendererAccessor::GetMinStreamVolumeSyncImpl,
            AudioRendererAccessor::GetMaxStreamVolume0Impl,
            AudioRendererAccessor::GetMaxStreamVolume1Impl,
            AudioRendererAccessor::GetMaxStreamVolumeSyncImpl,
            AudioRendererAccessor::GetUnderflowCount0Impl,
            AudioRendererAccessor::GetUnderflowCount1Impl,
            AudioRendererAccessor::GetUnderflowCountSyncImpl,
            AudioRendererAccessor::GetCurrentOutputDevices0Impl,
            AudioRendererAccessor::GetCurrentOutputDevices1Impl,
            AudioRendererAccessor::GetCurrentOutputDevicesSyncImpl,
            AudioRendererAccessor::SetChannelBlendModeImpl,
            AudioRendererAccessor::SetSilentModeAndMixWithOthersImpl,
            AudioRendererAccessor::GetSilentModeAndMixWithOthersImpl,
            AudioRendererAccessor::SetDefaultOutputDeviceImpl,
            AudioRendererAccessor::OnAudioInterruptImpl,
            AudioRendererAccessor::OnMarkReachImpl,
            AudioRendererAccessor::OffMarkReachImpl,
            AudioRendererAccessor::OnPeriodReachImpl,
            AudioRendererAccessor::OffPeriodReachImpl,
            AudioRendererAccessor::OnStateChangeImpl,
            AudioRendererAccessor::OnOutputDeviceChangeImpl,
            AudioRendererAccessor::OnOutputDeviceChangeWithInfoImpl,
            AudioRendererAccessor::OffOutputDeviceChangeImpl,
            AudioRendererAccessor::OffOutputDeviceChangeWithInfoImpl,
            AudioRendererAccessor::OnWriteDataImpl,
            AudioRendererAccessor::OffWriteDataImpl,
            AudioRendererAccessor::GetStateImpl,
        };
        return &AudioRendererAccessorImpl;
    }

    struct AudioRendererPeer {
        virtual ~AudioRendererPeer() = default;
    };
    const GENERATED_ArkUIAudioCapturerAccessor* GetAudioCapturerAccessor()
    {
        static const GENERATED_ArkUIAudioCapturerAccessor AudioCapturerAccessorImpl {
            AudioCapturerAccessor::DestroyPeerImpl,
            AudioCapturerAccessor::CtorImpl,
            AudioCapturerAccessor::GetFinalizerImpl,
            AudioCapturerAccessor::GetCapturerInfo0Impl,
            AudioCapturerAccessor::GetCapturerInfo1Impl,
            AudioCapturerAccessor::GetCapturerInfoSyncImpl,
            AudioCapturerAccessor::GetStreamInfo0Impl,
            AudioCapturerAccessor::GetStreamInfo1Impl,
            AudioCapturerAccessor::GetStreamInfoSyncImpl,
            AudioCapturerAccessor::GetAudioStreamId0Impl,
            AudioCapturerAccessor::GetAudioStreamId1Impl,
            AudioCapturerAccessor::GetAudioStreamIdSyncImpl,
            AudioCapturerAccessor::Start0Impl,
            AudioCapturerAccessor::Start1Impl,
            AudioCapturerAccessor::Read0Impl,
            AudioCapturerAccessor::Read1Impl,
            AudioCapturerAccessor::GetAudioTime0Impl,
            AudioCapturerAccessor::GetAudioTime1Impl,
            AudioCapturerAccessor::GetAudioTimeSyncImpl,
            AudioCapturerAccessor::Stop0Impl,
            AudioCapturerAccessor::Stop1Impl,
            AudioCapturerAccessor::Release0Impl,
            AudioCapturerAccessor::Release1Impl,
            AudioCapturerAccessor::GetBufferSize0Impl,
            AudioCapturerAccessor::GetBufferSize1Impl,
            AudioCapturerAccessor::GetBufferSizeSyncImpl,
            AudioCapturerAccessor::GetCurrentInputDevicesImpl,
            AudioCapturerAccessor::GetCurrentAudioCapturerChangeInfoImpl,
            AudioCapturerAccessor::GetOverflowCountImpl,
            AudioCapturerAccessor::GetOverflowCountSyncImpl,
            AudioCapturerAccessor::OnMarkReachImpl,
            AudioCapturerAccessor::OffMarkReachImpl,
            AudioCapturerAccessor::OnPeriodReachImpl,
            AudioCapturerAccessor::OffPeriodReachImpl,
            AudioCapturerAccessor::OnStateChangeImpl,
            AudioCapturerAccessor::OnAudioInterruptImpl,
            AudioCapturerAccessor::OffAudioInterruptImpl,
            AudioCapturerAccessor::OnInputDeviceChangeImpl,
            AudioCapturerAccessor::OffInputDeviceChangeImpl,
            AudioCapturerAccessor::OnAudioCapturerChangeImpl,
            AudioCapturerAccessor::OffAudioCapturerChangeImpl,
            AudioCapturerAccessor::OnReadDataImpl,
            AudioCapturerAccessor::OffReadDataImpl,
            AudioCapturerAccessor::GetStateImpl,
        };
        return &AudioCapturerAccessorImpl;
    }

    struct AudioCapturerPeer {
        virtual ~AudioCapturerPeer() = default;
    };
    const GENERATED_ArkUIAsrProcessingControllerAccessor* GetAsrProcessingControllerAccessor()
    {
        static const GENERATED_ArkUIAsrProcessingControllerAccessor AsrProcessingControllerAccessorImpl {
            AsrProcessingControllerAccessor::DestroyPeerImpl,
            AsrProcessingControllerAccessor::CtorImpl,
            AsrProcessingControllerAccessor::GetFinalizerImpl,
            AsrProcessingControllerAccessor::SetAsrAecModeImpl,
            AsrProcessingControllerAccessor::GetAsrAecModeImpl,
            AsrProcessingControllerAccessor::SetAsrNoiseSuppressionModeImpl,
            AsrProcessingControllerAccessor::GetAsrNoiseSuppressionModeImpl,
            AsrProcessingControllerAccessor::IsWhisperingImpl,
            AsrProcessingControllerAccessor::SetAsrVoiceControlModeImpl,
            AsrProcessingControllerAccessor::SetAsrVoiceMuteModeImpl,
            AsrProcessingControllerAccessor::SetAsrWhisperDetectionModeImpl,
            AsrProcessingControllerAccessor::GetAsrWhisperDetectionModeImpl,
        };
        return &AsrProcessingControllerAccessorImpl;
    }

    struct AsrProcessingControllerPeer {
        virtual ~AsrProcessingControllerPeer() = default;
    };
    const GENERATED_ArkUITonePlayerAccessor* GetTonePlayerAccessor()
    {
        static const GENERATED_ArkUITonePlayerAccessor TonePlayerAccessorImpl {
            TonePlayerAccessor::DestroyPeerImpl,
            TonePlayerAccessor::CtorImpl,
            TonePlayerAccessor::GetFinalizerImpl,
            TonePlayerAccessor::Load0Impl,
            TonePlayerAccessor::Load1Impl,
            TonePlayerAccessor::Start0Impl,
            TonePlayerAccessor::Start1Impl,
            TonePlayerAccessor::Stop0Impl,
            TonePlayerAccessor::Stop1Impl,
            TonePlayerAccessor::Release0Impl,
            TonePlayerAccessor::Release1Impl,
        };
        return &TonePlayerAccessorImpl;
    }

    struct TonePlayerPeer {
        virtual ~TonePlayerPeer() = default;
    };
    const GENERATED_ArkUIGlobalScope_ohos_multimedia_audioAccessor* GetGlobalScope_ohos_multimedia_audioAccessor()
    {
        static const GENERATED_ArkUIGlobalScope_ohos_multimedia_audioAccessor GlobalScope_ohos_multimedia_audioAccessorImpl {
            GlobalScope_ohos_multimedia_audioAccessor::DestroyPeerImpl,
            GlobalScope_ohos_multimedia_audioAccessor::GetAudioManagerImpl,
            GlobalScope_ohos_multimedia_audioAccessor::CreateAudioCapturer0Impl,
            GlobalScope_ohos_multimedia_audioAccessor::CreateAudioCapturer1Impl,
            GlobalScope_ohos_multimedia_audioAccessor::CreateAudioRenderer0Impl,
            GlobalScope_ohos_multimedia_audioAccessor::CreateAudioRenderer1Impl,
            GlobalScope_ohos_multimedia_audioAccessor::CreateTonePlayer0Impl,
            GlobalScope_ohos_multimedia_audioAccessor::CreateTonePlayer1Impl,
            GlobalScope_ohos_multimedia_audioAccessor::CreateAsrProcessingControllerImpl,
        };
        return &GlobalScope_ohos_multimedia_audioAccessorImpl;
    }

    struct GlobalScope_ohos_multimedia_audioPeer {
        virtual ~GlobalScope_ohos_multimedia_audioPeer() = default;
    };
    const GENERATED_ArkUIAccessors* GENERATED_GetArkUIAccessors()
    {
        static const GENERATED_ArkUIAccessors accessorsImpl = {
            GetAudioManagerAccessor,
            GetAudioRoutingManagerAccessor,
            GetAudioStreamManagerAccessor,
            GetAudioSessionManagerAccessor,
            GetAudioVolumeManagerAccessor,
            GetAudioVolumeGroupManagerAccessor,
            GetAudioSpatializationManagerAccessor,
            GetAudioRendererAccessor,
            GetAudioCapturerAccessor,
            GetAsrProcessingControllerAccessor,
            GetTonePlayerAccessor,
            GetGlobalScope_ohos_multimedia_audioAccessor,
        };
        return &accessorsImpl;
    }
    const GENERATED_ArkUIBasicNodeAPI* GENERATED_GetBasicAPI()
    {
        static const GENERATED_ArkUIBasicNodeAPI basicNodeAPIImpl = {
            GENERATED_ARKUI_BASIC_NODE_API_VERSION, // version
            OHOS::Ace::NG::GeneratedBridge::CreateNode,
            OHOS::Ace::NG::GeneratedApiImpl::GetNodeByViewStack,
            OHOS::Ace::NG::GeneratedApiImpl::DisposeNode,
            OHOS::Ace::NG::GeneratedApiImpl::DumpTreeNode,
            OHOS::Ace::NG::GeneratedApiImpl::AddChild,
            OHOS::Ace::NG::GeneratedApiImpl::RemoveChild,
            OHOS::Ace::NG::GeneratedApiImpl::InsertChildAfter,
            OHOS::Ace::NG::GeneratedApiImpl::InsertChildBefore,
            OHOS::Ace::NG::GeneratedApiImpl::InsertChildAt,
            OHOS::Ace::NG::GeneratedApiImpl::ApplyModifierFinish,
            OHOS::Ace::NG::GeneratedApiImpl::MarkDirty,
            OHOS::Ace::NG::GeneratedApiImpl::IsBuilderNode,
            OHOS::Ace::NG::GeneratedApiImpl::ConvertLengthMetricsUnit
        };
        return &basicNodeAPIImpl;
    }

    const GENERATED_ArkUIExtendedNodeAPI* GENERATED_GetExtendedAPI()
    {
        static const GENERATED_ArkUIExtendedNodeAPI extendedNodeAPIImpl = {
            GENERATED_ARKUI_EXTENDED_NODE_API_VERSION, // version
            OHOS::Ace::NG::GeneratedApiImpl::GetDensity,
            OHOS::Ace::NG::GeneratedApiImpl::GetFontScale,
            OHOS::Ace::NG::GeneratedApiImpl::GetDesignWidthScale,
            OHOS::Ace::NG::GeneratedApiImpl::SetCallbackMethod,
            OHOS::Ace::NG::GeneratedApiImpl::SetCustomMethodFlag,
            OHOS::Ace::NG::GeneratedApiImpl::GetCustomMethodFlag,
            OHOS::Ace::NG::GeneratedApiImpl::SetCustomCallback,
            OHOS::Ace::NG::GeneratedApiImpl::SetCustomNodeDestroyCallback,
            OHOS::Ace::NG::GeneratedApiImpl::MeasureLayoutAndDraw,
            OHOS::Ace::NG::GeneratedApiImpl::MeasureNode,
            OHOS::Ace::NG::GeneratedApiImpl::LayoutNode,
            OHOS::Ace::NG::GeneratedApiImpl::DrawNode,
            OHOS::Ace::NG::GeneratedApiImpl::SetAttachNodePtr,
            OHOS::Ace::NG::GeneratedApiImpl::GetAttachNodePtr,
            OHOS::Ace::NG::GeneratedApiImpl::SetMeasureWidth,
            OHOS::Ace::NG::GeneratedApiImpl::GetMeasureWidth,
            OHOS::Ace::NG::GeneratedApiImpl::SetMeasureHeight,
            OHOS::Ace::NG::GeneratedApiImpl::GetMeasureHeight,
            OHOS::Ace::NG::GeneratedApiImpl::SetX,
            OHOS::Ace::NG::GeneratedApiImpl::GetX,
            OHOS::Ace::NG::GeneratedApiImpl::SetY,
            OHOS::Ace::NG::GeneratedApiImpl::GetY,
            OHOS::Ace::NG::GeneratedApiImpl::GetLayoutConstraint,
            OHOS::Ace::NG::GeneratedApiImpl::SetAlignment,
            OHOS::Ace::NG::GeneratedApiImpl::GetAlignment,
            OHOS::Ace::NG::GeneratedApiImpl::IndexerChecker,
            OHOS::Ace::NG::GeneratedApiImpl::SetRangeUpdater,
            OHOS::Ace::NG::GeneratedApiImpl::SetLazyItemIndexer,
            OHOS::Ace::NG::GeneratedApiImpl::GetPipelineContext,
            OHOS::Ace::NG::GeneratedApiImpl::SetVsyncCallback,
            OHOS::Ace::NG::GeneratedApiImpl::SetChildTotalCount,
            OHOS::Ace::NG::GeneratedApiImpl::ShowCrash
        };
        return &extendedNodeAPIImpl;
    }

    // TODO: remove me!
    const GENERATED_ArkUIFullNodeAPI* GENERATED_GetFullAPI()
    {
        static const GENERATED_ArkUIFullNodeAPI fullAPIImpl = {
            GENERATED_ARKUI_FULL_API_VERSION, // version
            GENERATED_GetArkUINodeModifiers,
            GENERATED_GetArkUIAccessors,
            nullptr,
            OHOS::Ace::NG::GeneratedEvents::GENERATED_GetArkUiEventsAPI,
            OHOS::Ace::NG::GeneratedEvents::GENERATED_SetArkUiEventsAPI
        };
        return &fullAPIImpl;
    }

    void setLogger(const ServiceLogger* logger) {
        SetDummyLogger(reinterpret_cast<const GroupLogger*>(logger));
    }


    const GenericServiceAPI* GetServiceAPI()
    {
        static const GenericServiceAPI serviceAPIImpl = {
            GENERIC_SERVICE_API_VERSION, // version
            setLogger
        };
        return &serviceAPIImpl;
    }

    EXTERN_C IDLIZE_API_EXPORT const GENERATED_ArkUIAnyAPI* GENERATED_GetArkAnyAPI(
        GENERATED_Ark_APIVariantKind kind, int version)
    {
        switch (kind) {
            case GENERATED_FULL:
                if (version == GENERATED_ARKUI_FULL_API_VERSION)   {
                    return reinterpret_cast<const GENERATED_ArkUIAnyAPI*>(GENERATED_GetFullAPI());
                }
                break;
            case GENERATED_BASIC:
                if (version == GENERATED_ARKUI_BASIC_NODE_API_VERSION)   {
                    return reinterpret_cast<const GENERATED_ArkUIAnyAPI*>(GENERATED_GetBasicAPI());
                }
                break;
            case GENERATED_EXTENDED:
                if (version == GENERATED_ARKUI_EXTENDED_NODE_API_VERSION)   {
                    return reinterpret_cast<const GENERATED_ArkUIAnyAPI*>(GENERATED_GetExtendedAPI());
                }
                break;
            case GENERIC_SERVICE:
                if (version == GENERIC_SERVICE_API_VERSION)   {
                    return reinterpret_cast<const GENERATED_ArkUIAnyAPI*>(GetServiceAPI());
                }
                break;
            default:
                break;
        }
        return nullptr;
    }

}