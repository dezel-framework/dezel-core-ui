#ifndef DisplayBase_h
#define DisplayBase_h

#include <float.h>
#include <stddef.h>
#include <stdbool.h>

#define ABS_DBL_MIN -DBL_MAX
#define ABS_DBL_MAX +DBL_MAX

/**
 * @typedef DisplayRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueDisplay* DisplayRef;

/**
 * @typedef DisplayNodeRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueDisplayNode* DisplayNodeRef;

/**
 * @typedef PropertyRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueProperty* PropertyRef;

/**
 * @typedef ValueListRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueValueList* ValueListRef;

/**
 * @typedef ValueRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueValue* ValueRef;

/**
 * @typedef VariableValueRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueVariableValue* VariableValueRef;

/**
 * @typedef FunctionValueRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueFunctionValue* FunctionValueRef;

/**
 * @typedef StylesheetRef
 * @since 0.1.0
 * @hidden
 */
typedef struct OpaqueStylesheet* StylesheetRef;

/**
 * @typedef ParseError
 * @since 0.1.0
 * @hidden
 */
typedef struct {
	const char* message;
	const char* url;
	unsigned col;
	unsigned row;
} ParseError;

/**
 * @type DisplayNodeAnchorType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeAnchorTypeLength = 1
} DisplayNodeAnchorType;

/**
 * @type DisplayNodeAnchorUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeAnchorUnitPX = 1,
	kDisplayNodeAnchorUnitPC = 2,
	kDisplayNodeAnchorUnitVW = 3,
	kDisplayNodeAnchorUnitVH = 4,
	kDisplayNodeAnchorUnitPW = 5,
	kDisplayNodeAnchorUnitPH = 6,
	kDisplayNodeAnchorUnitCW = 7,
	kDisplayNodeAnchorUnitCH = 8
} DisplayNodeAnchorUnit;

/**
 * @type DisplayNodeSizeType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeSizeTypeFill = 1,
	kDisplayNodeSizeTypeWrap = 2,
	kDisplayNodeSizeTypeLength = 3
} DisplayNodeSizeType;

/**
 * @type DisplayNodeSizeUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeSizeUnitNone = 1,
	kDisplayNodeSizeUnitPX = 2,
	kDisplayNodeSizeUnitPC = 3,
	kDisplayNodeSizeUnitVW = 4,
	kDisplayNodeSizeUnitVH = 5,
	kDisplayNodeSizeUnitPW = 6,
	kDisplayNodeSizeUnitPH = 7,
	kDisplayNodeSizeUnitCW = 8,
	kDisplayNodeSizeUnitCH = 9
} DisplayNodeSizeUnit;

/**
 * @type DisplayNodePositionType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodePositionTypeAuto = 1,
	kDisplayNodePositionTypeLength = 2
} DisplayNodePositionType;

/**
 * @type DisplayNodePositionUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodePositionUnitNone = 1,
	kDisplayNodePositionUnitPX = 2,
	kDisplayNodePositionUnitPC = 3,
	kDisplayNodePositionUnitVW = 4,
	kDisplayNodePositionUnitVH = 5,
	kDisplayNodePositionUnitPW = 6,
	kDisplayNodePositionUnitPH = 7,
	kDisplayNodePositionUnitCW = 8,
	kDisplayNodePositionUnitCH = 9
} DisplayNodePositionUnit;

/**
 * @type DisplayNodeContentPositionType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeContentPositionTypeLength = 1
} DisplayNodeContentPositionType;

/**
 * @type DisplayNodeContentPositionUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeContentPositionUnitNone = 1,
	kDisplayNodeContentPositionUnitPX = 2
} DisplayNodeContentPositionUnit;

/**
 * @type DisplayNodeContentSize
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeContentSizeTypeAuto = 1,
	kDisplayNodeContentSizeTypeLength = 2,
} DisplayNodeContentSizeType;

/**
 * @type DisplayNodeContentSizeUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeContentSizeUnitNone = 1,
	kDisplayNodeContentSizeUnitPX = 2,
	kDisplayNodeContentSizeUnitPC = 3,
	kDisplayNodeContentSizeUnitVW = 4,
	kDisplayNodeContentSizeUnitVH = 5,
	kDisplayNodeContentSizeUnitPW = 6,
	kDisplayNodeContentSizeUnitPH = 7,
	kDisplayNodeContentSizeUnitCW = 8,
	kDisplayNodeContentSizeUnitCH = 9
} DisplayNodeContentSizeUnit;

/**
 * @type DisplayNodeContentDirection
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeContentDirectionVertical = 1,
	kDisplayNodeContentDirectionHorizontal = 2
} DisplayNodeContentDirection;

/**
 * @type DisplayNodeContentDisposition
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeContentDispositionStart = 1,
	kDisplayNodeContentDispositionCenter = 2,
	kDisplayNodeContentDispositionEnd = 3,
	kDisplayNodeContentDispositionSpaceAround = 4,
	kDisplayNodeContentDispositionSpaceBetween = 5,
	kDisplayNodeContentDispositionSpaceEvenly = 6
} DisplayNodeContentDisposition;

/**
 * @type DisplayNodeContentAlignment
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeContentAlignmentStart = 1,
	kDisplayNodeContentAlignmentCenter = 2,
	kDisplayNodeContentAlignmentEnd = 3,
} DisplayNodeContentAlignment;

/**
 * @type DisplayNodeBorderType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
    kDisplayNodeBorderTypeLength = 1
} DisplayNodeBorderType;

/**
 * @type DisplayNodeBorderUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
    kDisplayNodeBorderUnitPX = 1,
    kDisplayNodeBorderUnitPC = 2,
    kDisplayNodeBorderUnitVW = 3,
    kDisplayNodeBorderUnitVH = 4,
	kDisplayNodeBorderUnitPW = 5,
	kDisplayNodeBorderUnitPH = 6,
	kDisplayNodeBorderUnitCW = 7,
	kDisplayNodeBorderUnitCH = 8
} DisplayNodeBorderUnit;

/**
 * @type DisplayNodeMarginType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeMarginTypeLength = 1
} DisplayNodeMarginType;

/**
 * @type DisplayNodeMarginUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodeMarginUnitPX = 1,
	kDisplayNodeMarginUnitPC = 2,
	kDisplayNodeMarginUnitVW = 3,
	kDisplayNodeMarginUnitVH = 4,
	kDisplayNodeMarginUnitPW = 5,
	kDisplayNodeMarginUnitPH = 6,
	kDisplayNodeMarginUnitCW = 7,
	kDisplayNodeMarginUnitCH = 8
} DisplayNodeMarginUnit;

/**
 * @type DisplayNodePaddingType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodePaddingTypeLength = 1
} DisplayNodePaddingType;

/**
 * @type DisplayNodePaddingUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kDisplayNodePaddingUnitPX = 1,
	kDisplayNodePaddingUnitPC = 2,
	kDisplayNodePaddingUnitVW = 3,
	kDisplayNodePaddingUnitVH = 4,
	kDisplayNodePaddingUnitPW = 5,
	kDisplayNodePaddingUnitPH = 6,
	kDisplayNodePaddingUnitCW = 7,
	kDisplayNodePaddingUnitCH = 8
} DisplayNodePaddingUnit;

/**
 * @typedef MeasuredSize
 * @since 0.1.0
 * @hidden
 */
typedef struct {
	double width;
	double height;
} MeasuredSize;

/**
 * @typedef ValueType
 * @since 0.1.0
 * @hidden
 */
typedef enum {
	kValueTypeNull = 1,
	kValueTypeString = 2,
	kValueTypeNumber = 3,
	kValueTypeBoolean = 4,
	kValueTypeFunction = 5,
	kValueTypeVariable = 6
} ValueType;

/**
 * @typedef ValueUnit
 * @since 0.1.0
 * @hidden
 */
typedef enum  {
	kValueUnitNone = 1,
	kValueUnitPX = 2,
	kValueUnitPC = 3,
	kValueUnitVW = 4 ,
	kValueUnitVH = 5,
	kValueUnitPW = 6,
	kValueUnitPH = 7,
	kValueUnitCW = 8,
	kValueUnitCH = 9,
	kValueUnitDeg = 10,
	kValueUnitRad = 11
} ValueUnit;

/**
 * @typedef DisplayResolveCallback
 * @since 0.1.0
 * @hidden
 */
typedef void (*DisplayCallback)(DisplayRef display);

/**
 * @typedef DisplayNodeMeasureCallback
 * @since 0.1.0
 * @hidden
 */
typedef void (*DisplayNodeMeasureCallback)(DisplayNodeRef node, MeasuredSize* size, double w, double h, double minw, double maxw, double minh, double maxh);

/**
 * @typedef DisplayNodeResolveCallback
 * @since 0.1.0
 * @hidden
 */
typedef void (*DisplayNodeCallback)(DisplayNodeRef node);

/**
 * @typedef DisplayNodeMeasureCallback
 * @since 0.1.0
 * @hidden
 */
typedef void (*DisplayNodeMeasureCallback)(DisplayNodeRef node, MeasuredSize* size, double w, double h, double minw, double maxw, double minh, double maxh);

/**
 * @typedef DisplayNodeUpdateCallback
 * @since 0.1.0
 * @hidden
 */
typedef void (*DisplayNodeUpdateCallback)(DisplayNodeRef node, PropertyRef property, const char* name);

#endif
