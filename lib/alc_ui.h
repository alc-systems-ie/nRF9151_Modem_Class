#pragma once

#include <dk_buttons_and_leds.h>

namespace alc::ui
{
  class Ui
  {
    private:
      constexpr static int M_OK { 0 };

    public:
      Ui(void);

      static void SetLed1(bool flag);
      static void SetLed2(bool flag);
      static void SetLed3(bool flag);
      static void SetLed4(bool flag);

      bool GetButton1(void);
      bool GetButton2(void);
      bool GetButton3(void);
      bool GetButton4(void);

      void ClearButton1(void);
      void ClearButton2(void);
      void ClearButton3(void);
      void ClearButton4(void);

    private:
      static void buttonHandler(uint32_t buttonState, uint32_t hasChanged);
  };
}
