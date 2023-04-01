
enum CoreEventType {

    //  no event, shouldn't be used
    none,               

    //  focus events
    focusGain,
    focusLose,

    //  basic keyboard events
    keyText,
    keyRaw,
    keyEsc,
    keyEnter,

    //  basic pointer events
    pointerUp,
    pointerDown,
    pointerEnter,
    pointerExit,
    pointerMove,

    //  advanced pointer events
    pointerDownLong,
    pointerDrag,


    //  drag drop events
    dragDropEnter,
    dragDropExit,
    dragDropRelease
};    
