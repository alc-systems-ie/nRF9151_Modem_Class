#include <string>
#include <zephyr/logging/log.h>
#include "lib/alc_ui.h"

LOG_MODULE_REGISTER(alc_ui, LOG_LEVEL_INF);

namespace alc::ui
{
  static std::string m_message { "Hello Everyone!" };
  static bool m_btn_1_pressed { false };
  static bool m_btn_2_pressed { false };
  static bool m_btn_3_pressed { false };
  static bool m_btn_4_pressed { false };
  
  void Ui::buttonHandler(uint32_t buttonState, uint32_t hasChanged)
  {
	  switch (hasChanged) 
    {
	    case DK_BTN1_MSK:
		    if (buttonState & DK_BTN1_MSK) { LOG_INF("Button 1 pressed."); }
	    break;
	    case DK_BTN2_MSK:
		    if (buttonState & DK_BTN2_MSK) { LOG_INF("Button 2 pressed."); }
	    break;
	    case DK_BTN3_MSK:
		    if (buttonState & DK_BTN3_MSK) { LOG_INF("Button 3 pressed."); }
	    break;
	    case DK_BTN4_MSK:
		    if (buttonState & DK_BTN4_MSK) { LOG_INF("Button 4 pressed."); }
	    break;
	  }
  }

  Ui::Ui(void)
  {
    int result = dk_leds_init();
    if (result != M_OK) { LOG_ERR("Failed to initialise LED library. Error: %d", result); }

	  result = dk_buttons_init(buttonHandler);
		if (result != M_OK) { LOG_ERR("Failed to initialise the buttons library: %d", result); }
  }


  void Ui::SetLed1(bool flag)
  {
    (flag == true) ? dk_set_led_on(DK_LED1) : dk_set_led_off(DK_LED1);
  }

  void Ui::SetLed2(bool flag)
  {
    (flag == true) ? dk_set_led_on(DK_LED2) : dk_set_led_off(DK_LED2);
  }

  void Ui::SetLed3(bool flag)
  {
    (flag == true) ? dk_set_led_on(DK_LED3) : dk_set_led_off(DK_LED3);
  }

  void Ui::SetLed4(bool flag)
  {
    (flag == true) ? dk_set_led_on(DK_LED4) : dk_set_led_off(DK_LED4);
  }

  bool Ui::GetButton1(void) { return m_btn_1_pressed; }

  bool Ui::GetButton2(void) { return m_btn_2_pressed; }

  bool Ui::GetButton3(void) { return m_btn_3_pressed; }

  bool Ui::GetButton4(void) { return m_btn_4_pressed; }

  void Ui::ClearButton1(void) { m_btn_1_pressed = false; }  

  void Ui::ClearButton2(void) { m_btn_2_pressed = false; }  

  void Ui::ClearButton3(void) { m_btn_3_pressed = false; }  

  void Ui::ClearButton4(void) { m_btn_4_pressed = false; }  
}
