#include <zephyr/logging/log.h>
#include "lib/alc_modem.h"
#include "lib/alc_ui.h"


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

alc::modem::Modem modem;
alc::ui::Ui ui;

int main(void)
{
	modem.InitAndConnect();

  modem.ShowEvalAll();


  while (true)
  {
    k_msleep(20);
  }
	return 0;
}
