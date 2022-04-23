#include "InputHandling.h"

Button ButtonUpload(PIN_BTN_UPLOAD, DEBOUNCE_TIME, buttonChangedUpload);

void buttonChangedUpload(int state)
{
    ButtonUpload.buttonChanged(state);
}