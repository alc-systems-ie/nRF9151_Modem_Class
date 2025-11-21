#include <cerrno>
#include <zephyr/logging/log.h>
#include <modem/nrf_modem_lib.h>
#include <modem/modem_info.h>
#include <modem/lte_lc.h>
#include "lib/alc_modem.h"
#include "lib/alc_ui.h"

LOG_MODULE_REGISTER(alc_modem, LOG_LEVEL_INF);

namespace alc::modem
{

  void Modem::lteHandler(const struct lte_lc_evt* const lteEvent)
  {
    const auto type { lteEvent->type };
    const auto regStatus { lteEvent->nw_reg_status };
    const auto rrcMode { lteEvent->rrc_mode };

    switch (type) 
    {
      // 0. Network Registration Status.
      case LTE_LC_EVT_NW_REG_STATUS:
        LOG_INF("LTE LC Event - Network Registration Status Change.");
        // 0. Not Registered.
        if (regStatus == LTE_LC_NW_REG_NOT_REGISTERED) { LOG_INF("LTE_LC_EVT_REG_STATUS: Not Registered - %d", regStatus); }
        // 1. Home.
        if (regStatus == LTE_LC_NW_REG_REGISTERED_HOME) { LOG_INF("LTE_LC_EVT_REG_STATUS: Registered Home - %d", regStatus); }
        // 2. Searching.
        if (regStatus == LTE_LC_NW_REG_SEARCHING) { LOG_INF("LTE_LC_EVT_REG_STATUS: Searching - %d", regStatus); }
        // 3. Denied.
        if (regStatus == LTE_LC_NW_REG_REGISTRATION_DENIED) { LOG_INF("LTE_LC_EVT_REG_STATUS: Denied - %d", regStatus); }
        // 4. Unknown.
        if (regStatus == LTE_LC_NW_REG_UNKNOWN) { LOG_INF("LTE_LC_EVT_REG_STATUS: Unknown - %d", regStatus); }
        // 5. Roaming.
        if (regStatus == LTE_LC_NW_REG_REGISTERED_ROAMING) { LOG_INF("LTE_LC_EVT_REG_STATUS: Roaming - %d", regStatus); }
        // 50. RX Only, Not Registered.
        // if (regStatus == LTE_LC_NW_REG_RX_ONLY_NOT_REGISTERED) { LOG_INF("LTE_LC_EVT_REG_STATUS: RX Only, Not Registered - %d", regStatus); }
        // 51. RX Only, Registered Home.
        // if (regStatus == LTE_LC_NW_REG_RX_ONLY_REGISTERED_HOME) { LOG_INF("LTE_LC_EVT_REG_STATUS: RX Only, Home - %d", regStatus); }
        // 52. RX Only, Searching.
        // if (regStatus == LTE_LC_NW_REG_RX_ONLY_SEARCHING) { LOG_INF("LTE_LC_EVT_REG_STATUS: RX Only, Searching - %d", regStatus); }
        // 53. RX Only, Denied.
        // if (regStatus == LTE_LC_NW_REG_RX_ONLY_REGISTRATION_DENIED) { LOG_INF("LTE_LC_EVT_REG_STATUS: RX Only, Denied - %d", regStatus); }
        // 54. RX Only, Unknown.
        // if (regStatus == LTE_LC_NW_REG_RX_ONLY_UNKNOWN) { LOG_INF("LTE_LC_EVT_REG_STATUS: RX Only, Unknown- %d", regStatus); }
        // 55. RX Only, Roaming.
        // if (regStatus == LTE_LC_NW_REG_RX_ONLY_REGISTERED_ROAMING) { LOG_INF("LTE_LC_EVT_REG_STATUS: RX Only, Roaming - %d", regStatus); }
        // 90. UICC Fail.
        if (regStatus == LTE_LC_NW_REG_UICC_FAIL) { LOG_INF("LTE_LC_EVT_REG_STATUS: UICC Fail - %d", regStatus); }
        // 91. No Suitable Cell.
        // if (regStatus == LTE_LC_NW_REG_NO_SUITABLE_CELL) { LOG_INF("LTE_LC_EVT_REG_STATUS:  No Suitable Cell - %d", regStatus); }

        if ((regStatus == LTE_LC_NW_REG_REGISTERED_HOME) || (regStatus == LTE_LC_NW_REG_REGISTERED_ROAMING)) 
        { 
          m_reg_status = regStatus;
		      LOG_INF("Connection - %s", (regStatus == LTE_LC_NW_REG_REGISTERED_HOME ? "Home Network" : "Roaming"));
     	    k_sem_give(&m_sem_lte_connected);
        }
        break;

      // 1. PSM Update. (Requires CONFIG_LTE_LC_PSM_MODULE).
      case LTE_LC_EVT_PSM_UPDATE:
        LOG_INF("LTE LC Event - PSM Update.");
        break;

      // 2. eDRX Update. (Requires CONFIG_LTE_LC_EDRX_MODULE).
      case LTE_LC_EVT_EDRX_UPDATE:
        LOG_INF("LTE LC Event - eDRX Update.");
        break;

      // 3. RRC Status.
	    case LTE_LC_EVT_RRC_UPDATE:
		     LOG_INF("LTE LC Event - RRC Mode Update: %s", (rrcMode == LTE_LC_RRC_MODE_CONNECTED ? "Connected" : "Idle"));
         (rrcMode == LTE_LC_RRC_MODE_CONNECTED) ? alc::ui::Ui::SetLed2(true) : alc::ui::Ui::SetLed2(false);
		    break;

      // 4. Cell Update.
      case LTE_LC_EVT_CELL_UPDATE:
        LOG_INF("LTE LC Event - Cell Update.");
        break;

      // 5. Mode Update.
      case LTE_LC_EVT_LTE_MODE_UPDATE:
        LOG_INF("LTE LC Event - LTE Mode Update.");
        break;

      // 6. Tau Pre-Warning. (Reuires CONFIG_LTE_LC_TAU_PRE_WARNING_MODULE).
      case LTE_LC_EVT_TAU_PRE_WARNING:
        LOG_INF("LTE LC Event - TAU Pre-warning.");
        break;

      // 7. Neighbour Cell Measurement. (Requires CONFIG_LTE_LC_NEIGHBOR_CELL_MEAS_MODULE).
      case LTE_LC_EVT_NEIGHBOR_CELL_MEAS:
        LOG_INF("LTE LC Event - Neighbour Cell Measurment.");
        break;

      // 8. Modem Sleep Pre-Warning. (Requires CONFIG_LTE_LC_MODEM_SLEEP_MODULE).
      case LTE_LC_EVT_MODEM_SLEEP_EXIT_PRE_WARNING:
        LOG_INF("LTE LC Event - Modem Sleep Exit Pre-Warning.");
        break;

      // 9. Modem Sleep Exit. (Requires CONFIG_LTE_LC_MODEM_SLEEP_MODULE).
      case LTE_LC_EVT_MODEM_SLEEP_EXIT:
        LOG_INF("LTE LC Event - Modem Sleep Exit.");
        break;

      // 10. Modem Sleep Enter. (Requires CONFIG_LTE_LC_MODEM_SLEEP_MODULE).
      case LTE_LC_EVT_MODEM_SLEEP_ENTER:
        LOG_INF("LTE LC Event - Modem Sleep Enter.");
        break;

      // 11. Modem Event.
      case LTE_LC_EVT_MODEM_EVENT:
        LOG_INF("LTE LC Event - Modem Event.");
        break;

      // 12. Release Assistance Indication Update. (Requires CONFIG_LTE_LC_RAI_MODULE.)
      case LTE_LC_EVT_RAI_UPDATE:
        LOG_INF("LTE LC Event - RAI Update.");
        break;

      // 13. Environment Evaluation Result. v2.0.3 only.   
      // case LTE_LC_EVT_ENV_EVAL_RESULT:
        // LOG_INF("Environment Evaluation Result.");
        // break;

      // Default
      default:
        break;
    }
  }

  ////////////////////////////////////////////
  // Connection Evaluation Parameter Getters.
  ////////////////////////////////////////////

  void Modem::ShowEvalAll(void)
  {
    if (!validParameters()) { return; }

    GetConnEvalRrcMode();
    GetConnEvalEnergyEstimate();
    GetConnEvalTauTriggered();
    GetConnEvalCoverageEnhancement();
    GetConnEvalEarfcn();
    GetConnEvalDlPathloss();
    GetConnEvalRsrp();
    GetConnEvalRsrq();
    GetConnEvalTxRepetitions();
    GetConnEvalRxRepetitions();
    GetConnEvalPhysicalCellId();
    GetConnEvalBand();
    GetConnEvalSNR();
    GetConnEvalTxPower();
    GetConnEvalMcc();
    GetConnEvalMnc();
    GetConnEvalEutranCellId();
  }

  std::optional<lte_lc_rrc_mode> Modem::GetConnEvalRrcMode(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_IDLE) { LOG_INF("Eval: RRC Mode = Idle."); }
    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_CONNECTED) { LOG_INF("Eval: RRC Mode = Connected."); }

    return m_conn_eval_params->rrc_state;
  }
  
  std::optional<lte_lc_energy_estimate> Modem::GetConnEvalEnergyEstimate(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->energy_estimate == LTE_LC_ENERGY_CONSUMPTION_NORMAL) { LOG_INF("Eval: Energy Consumption = Normal."); }
    if (m_conn_eval_params->energy_estimate == LTE_LC_ENERGY_CONSUMPTION_REDUCED) { LOG_INF("Eval: Energy Consumption = Reduced."); }
    if (m_conn_eval_params->energy_estimate == LTE_LC_ENERGY_CONSUMPTION_EFFICIENT) { LOG_INF("Eval: Energy Consumption = Efficient."); }
    if (m_conn_eval_params->energy_estimate == LTE_LC_ENERGY_CONSUMPTION_EXCESSIVE) { LOG_WRN("Eval: Energy Consumption = Excessive."); }
    if (m_conn_eval_params->energy_estimate == LTE_LC_ENERGY_CONSUMPTION_INCREASED) { LOG_WRN("Eval: Energy Consumption = Increased."); }

    return m_conn_eval_params->energy_estimate;
  }

  std::optional<lte_lc_tau_triggered> Modem::GetConnEvalTauTriggered(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->tau_trig == LTE_LC_CELL_IN_TAI_LIST) { LOG_INF("Eval: Cell is in Traffic Area Identifier List."); }
    if (m_conn_eval_params->tau_trig == LTE_LC_CELL_NOT_IN_TAI_LIST) { LOG_WRN("Eval: Cell is NOT in Traffic Area Identifier List."); }
    if (m_conn_eval_params->tau_trig == LTE_LC_CELL_UNKNOWN) { LOG_INF("Eval: Cell is unknown."); }

    return m_conn_eval_params->tau_trig;
  }

  std::optional<lte_lc_ce_level> Modem::GetConnEvalCoverageEnhancement(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->ce_level == LTE_LC_CE_LEVEL_0) { LOG_INF("Eval: Coverage Enhancement Level = 0."); }
    if (m_conn_eval_params->ce_level == LTE_LC_CE_LEVEL_1) { LOG_INF("Eval: Coverage Enhancement Level = 1."); }
    if (m_conn_eval_params->ce_level == LTE_LC_CE_LEVEL_2) { LOG_INF("Eval: Coverage Enhancement Level = 2."); }
    if (m_conn_eval_params->ce_level == LTE_LC_CE_LEVEL_3) { LOG_INF("Eval: Coverage Enhancement Level = 3."); }
    if (m_conn_eval_params->ce_level == LTE_LC_CE_LEVEL_UNKNOWN) { LOG_WRN("Eval: Coverage Enhancement Level Unknown."); }

    return m_conn_eval_params->ce_level;
  }

  std::optional<int> Modem::GetConnEvalEarfcn(void)
  {
    if (!validParameters()) { return std::nullopt; }
    
    LOG_INF("Eval: EARFCN (Frequency Channel Number) = %d.", m_conn_eval_params->earfcn);

    return m_conn_eval_params->earfcn;
  }

  std::optional<int16_t> Modem::GetConnEvalDlPathloss(void)
  {
    if (!validParameters()) { return std::nullopt; }

    LOG_INF("Eval: Reduction in power density = %d dB.", m_conn_eval_params->dl_pathloss);

    return m_conn_eval_params->dl_pathloss;
  }

  std::optional<int16_t> Modem::GetConnEvalRsrp(void)
  {
    if (!validParameters()) { return std::nullopt; }

    LOG_INF("Eval: RSRP (Reference Signal Received Power) = %d dBm.", RSRP_IDX_TO_DBM(m_conn_eval_params->rsrp));

    return m_conn_eval_params->rsrp;
  }

  std::optional<int16_t> Modem::GetConnEvalRsrq(void)
  {
    if (!validParameters()) { return std::nullopt; }

    LOG_INF("Eval: RSRQ (Reference Signal Received Quality) = %d dB.", static_cast<int>(RSRQ_IDX_TO_DB(m_conn_eval_params->rsrq)));

    return m_conn_eval_params->rsrq;
  
  }

  std::optional<int16_t> Modem::GetConnEvalTxRepetitions(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_IDLE) 
    {
      LOG_INF("Eval: Tx Repetitions - PRACH (Physical Random Access Channel) = %d.", m_conn_eval_params->tx_rep);
    }

    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_CONNECTED) 
    {
      LOG_INF("Eval: Tx Repetitions - PUSCH (Physical Uplink Shared Channel) = %d.", m_conn_eval_params->tx_rep);
    }

    return m_conn_eval_params->tx_rep;
  }

  std::optional<int16_t> Modem::GetConnEvalRxRepetitions(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_IDLE) 
    {
      LOG_INF("Eval: RX Repetitions - PRACH (Physical Random Access Channel) = %d dBm.", m_conn_eval_params->rx_rep);
    }

    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_CONNECTED) 
    {
      LOG_INF("Eval: Rx Repetitions - PUSCH (Physical Uplink Shared Channel) = %d dBm.", m_conn_eval_params->rx_rep);
    }

    return m_conn_eval_params->rx_rep;
  }

  std::optional<int16_t> Modem::GetConnEvalPhysicalCellId(void)
  {
    if (!validParameters()) { return std::nullopt; }

    LOG_INF("Eval: Physical Cell Id = %d.", m_conn_eval_params->phy_cid);

    return m_conn_eval_params->phy_cid;
  }

  std::optional<int16_t> Modem::GetConnEvalBand(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->band == 0) 
    {  
      LOG_WRN("Eval: Band information not available.");
      return m_conn_eval_params->band;
    }

    LOG_INF("Eval: Band = %d.", m_conn_eval_params->band);
    return m_conn_eval_params->band;
  }

  std::optional<int16_t> Modem::GetConnEvalSNR(void)
  {
    if (!validParameters()) { return std::nullopt; }

    LOG_INF("Eval: SNR = %d.", m_conn_eval_params->snr);
    return m_conn_eval_params->snr;
  }

  std::optional<int16_t> Modem::GetConnEvalTxPower(void)
  {
    if (!validParameters()) { return std::nullopt; }

    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_IDLE) 
    {
      LOG_INF("Eval: Tx Power - PRACH (Physical Random Access Channel) = %d dBm.", m_conn_eval_params->tx_power);
    }

    if (m_conn_eval_params->rrc_state == LTE_LC_RRC_MODE_CONNECTED) 
    {
      LOG_INF("Eval: Tx Power - PUSCH (Physical Uplink Shared Channel) = %d dBm.", m_conn_eval_params->tx_power);
    }

    return m_conn_eval_params->tx_power;
  }
  
  std::optional<int> Modem::GetConnEvalMcc(void)
  {
    if (!validParameters()) { return std::nullopt; }

    const char* country { };

    if (m_conn_eval_params->mcc == M_MCC_IRELAND) { country = M_IRELAND; }

    LOG_INF("Eval: MCC = %d - %s", m_conn_eval_params->mcc, country);

    return m_conn_eval_params->mcc;
  }

  std::optional<int> Modem::GetConnEvalMnc(void)
  {
    if (!validParameters()) { return std::nullopt; }

    const char* netz { "Unknown" };
    if (m_conn_eval_params->mnc == M_MNC_VODAFONE) { netz = M_VODAFONE; }
    if (m_conn_eval_params->mnc == M_MNC_O2) { netz = M_O2; }
    if (m_conn_eval_params->mnc == M_MNC_EIR) { netz = M_EIR; }
    if (m_conn_eval_params->mnc == M_MNC_THREE) { netz = M_THREE; }

    LOG_INF("Eval: MNC = 0%d - %s", m_conn_eval_params->mnc, netz);

    return m_conn_eval_params->mnc;
  }

  std::optional<uint32_t> Modem::GetConnEvalEutranCellId(void)
  {
    if (!validParameters()) { return std::nullopt; }

    LOG_INF("Eval: Cell Id = %d", m_conn_eval_params->cell_id);

    return m_conn_eval_params->cell_id;
  }

  //  NOT YET RELEASED!
  //
  // alc_error_t Modem::MeasureEnviroment(void)
  // {
  //   // Receiver must be in RX Mode.
  //   // Results are signalled by LTE_LC_EVT_ENV_EVAL_RESULT.
  //   if (LTE_LC_FUNC_MODE_RX_ONLY != GetFuncMode())
  //   {
  //     LOG_ERR("Ensure Function Mode is RX_Only before calling MeasureEnvironment.");
  //     return alc_error_t::ENV_EVAL_MODE_ERROR;
  //   }
  //   lte_lc_env_eval_params* params { nullptr };
  //   int result { lte_lc_env_eval(params) };
  //   if (0 != result)
  //   {
  //     return alc_error_t::FAIL;
  //   }
  //
  //   return alc_error_t::OK;
  // }
  //

  bool Modem::validParameters(void)
  {
    constexpr int OK { 0 };
    if (m_conn_eval_param_flag == false) 
    {
      int result { getConnectionEvaluationParameters() };
      if (result != OK) { return false; }
    }

    return true;
  }

  int Modem::getConnectionEvaluationParameters(void)
  {
    constexpr int OK { 0 };
    /* Note, assignment to temp is required here as m_conn_eval_params 
    is std::optional and cannot be passed directly to lte_lc_conn_eval_params_get(). */
    lte_lc_conn_eval_params temp { };  
    m_conn_eval_param_flag = false;

    int result { lte_lc_conn_eval_params_get(&temp) };
    if (result != OK) 
    { 
      LOG_ERR("Failed to get evaluation parameters.");
      return result;
    }

    m_conn_eval_params = temp;
    LOG_INF("Evaluation parameters obtained.");
    m_conn_eval_param_flag = true;
    return result;
  }
  
  Modem::Modem(void)
  {
    // m_reg_status = std::nullopt;
  }

  bool Modem::initModem(void)
  {
	  int result { nrf_modem_lib_init() };
	  if (result) {
		  LOG_ERR("Failed to initialise the modem library, error: %d.", result);
      return false;
	  }
	  LOG_INF("Modem library initialised.");
    return true;
  }

  alc_error_t Modem::ConnectAsync(void)
  {
	  int result { lte_lc_connect_async(lteHandler) };
	  if (result != 0 ) {
		  LOG_ERR("Error in lte_lc_connect_async, error: %d", result);
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      if (result == -EINVAL) { return alc_error_t::NO_HANDLER; } 
      // Catch-all.
      return alc_error_t::CONNECT_FAIL; 
	  }

	  k_sem_take(&m_sem_lte_connected, K_FOREVER);

	  LOG_INF("Connected to LTE network.");
    return alc_error_t::OK;
  }

  alc_error_t Modem::InitAndConnect(void)
  {
    constexpr int init { 0 };
    constexpr int count { 1 };
    k_sem_init(&m_sem_lte_connected, init, count);

    // Initialise Modem.
    if (!initModem()) { return alc_error_t::INIT_FAIL; }
	
    // Connect to LTE Network.
    if (alc_error_t::OK != ConnectAsync()) { return alc_error_t::CONNECT_FAIL; }

    // LED shows successful connection.
    alc::ui::Ui::SetLed1(true);

  	return alc_error_t::OK;
  }

  alc_error_t Modem::RegisterHandler(void)
  {
    lte_lc_register_handler(lteHandler);  
    return alc_error_t::OK;
  }

  alc_error_t Modem::DeregisterHandler(void)
  {
    lte_lc_deregister_handler(lteHandler);  
    return alc_error_t::OK;
  }

  alc_error_t Modem::SetModemOffline(void)
  {
    int result { lte_lc_offline() };
    if (0 != result) 
    {
      LOG_ERR("Unable to set modem offline. Error: %d.", result);
      if (-EFAULT == result) return alc_error_t::OFFLINE_FAIL;
      // Catch-all.
      return alc_error_t::FAIL;
    }
      
    return alc_error_t::OK;
  }

  alc_error_t Modem::SetModemPowerOff(void)
  {
    int result { lte_lc_power_off() };
    if (0 != result)
    {
      if (result == -EFAULT) { return alc_error_t::POWER_OFF_FAIL; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }

    return alc_error_t::OK;
  }

  alc_error_t Modem::SetModemNormal(void)
  {
    int result { lte_lc_normal() };
    if (0 != result)
    {
      if (result == -EFAULT) { return alc_error_t::NORMAL_FAIL; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
  
    return alc_error_t::OK;
  }

  alc_error_t Modem::SetPsmParams(const std::string rpTau, const std::string rat)
  {
    // RPTAU = Requested Periodic TAU.
    // RAT = Requested Active Timeparblock Requested periodic TAU as a null-terminated 8 character long bit field string.  
    
    // RPTAU:
    // For example, value of 32400 s is represented as '00101001'.  
    // Bits 5 to 1 represent the binary coded timer value that is multiplied by timer unit.  
    // Bits 8 to 6 define the timer unit as follows:  
    // \- 000: 10 minutes  
    // \- 001: 1 hour  
    // \- 010: 10 hours  
    // \- 011: 2 seconds  
    // \- 100: 30 seconds  
    // \- 101: 1 minute  
    // \- 110: 320 hours  
   
    // RAT
    // Set to @c NULL to use modem's default value.  
    // See 3GPP 24.008 Ch. 10.5.7.3 for data format.  
    // For example, value of 120 s is represented as '00100010'.  
    // Bits 5 to 1 represent the binary coded timer value that is multiplied by timer unit.  
    // Bits 8 to 6 define the timer unit as follows:  
    // \- 000: 2 seconds  
    // \- 001: 1 minute  
    // \- 010: 6 minutes  
    // \- 111: Timer is deactivated  
    int result { lte_lc_psm_param_set(rpTau.c_str(), rat.c_str()) };
    if (0 != result)
    {
      LOG_ERR("Unable to set PSM parameters. Error: %d.", result);
      if (result == -EINVAL) return alc_error_t::PSM_PARAM_ERROR;
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t SetPsmParamsSeconds(const int rpTau, const int rat)
  {
    int result { lte_lc_psm_param_set_seconds(rpTau, rat) };
    if (0 != result)
    {
      LOG_ERR("Unable to set PSM parameters. Error: %d.", result);
      if (result == -EINVAL) return alc_error_t::PSM_PARAM_ERROR;
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::ProprietaryPsmRequest(const bool flag)
  {
    int result { lte_lc_proprietary_psm_req(flag) };
    if (0 != result)
    {
      LOG_ERR("Failed to Enable / Disable PSM. Error: %d.", result);
      if (result == -EFAULT) return alc_error_t::AT_COMMAND_FAIL;
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::PsmRequest(const bool flag)
  {
    int result { lte_lc_psm_req(flag) };
    if (0 != result)
    {
      LOG_ERR("Failed to Enable / Disable PSM. Error: %d.", result);
      if (result == -EFAULT) return alc_error_t::AT_COMMAND_FAIL;
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  std::optional<int> Modem::GetPsmRptau(void)
  {
    if (alc_error_t::OK == getPsmData()) { return m_psm_rptau;  }

    return std::nullopt;
  }

  std::optional<int> Modem::GetPsmRat(void)
  {
    if (alc_error_t::OK == getPsmData()) { return m_psm_rat;  }
  
    return std::nullopt;
  }

  alc_error_t Modem::SetEdrxPtw(const std::string ptw)
  {
    // Sets the eDRX Paging Time Window.
    // Refer to manual prior to use, since this is usually set to default.
    int result { lte_lc_ptw_set(m_lte_mode, ptw.c_str()) };
    if (0 != result)
    {
      LOG_ERR("Failed to set eDrx Paging Time Window Error: %d.", result);
      if (result == -EINVAL) return alc_error_t::EDRX_PARAM_ERRROR;
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::SetEdrxParam(const std::string param)
  {
    // Requires CONFIG_LTE_LC_EDRX_MODULE to be enabled.
    int result { lte_lc_edrx_param_set(m_lte_mode, param.c_str()) };
    if (0 != result)
    {
      LOG_ERR("Failed to set eDrx parameter. Error: %d.", result);
      if (result == -EINVAL) return alc_error_t::EDRX_PARAM_ERRROR;
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::EdrxRequest(const bool flag)
  {
    // Requires CONFIG_LTE_LC_EDRX_MODULE to be enabled.
    int result { lte_lc_edrx_req(flag) };
    if (0 != result)
    {
      LOG_ERR("Failed to errot eDrx.  Error: %d.", result);
      if (result == -EFAULT) return alc_error_t::AT_COMMAND_FAIL;
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::EdrxGet(void)
  {
    // Updates eDrx related data members.
    lte_lc_edrx_cfg* edrxData { }; 
    m_edrx_value = std::nullopt;
    m_edrx_ptw = std::nullopt;
    int result { lte_lc_edrx_get(edrxData) };

    if (0 != result)
    {
      LOG_ERR("Failed to get eDrx data.  Error: %d.", result);
      if (result == -EINVAL) return alc_error_t::EDRX_PARAM_ERROR;
      if (result == -EFAULT) return alc_error_t::AT_COMMAND_FAIL;
      if (result == -EBADMSG) return alc_error_t::AT_COMMAND_FAIL;
      // Catch-all.
      return alc_error_t::FAIL;
    }

    m_lte_mode = edrxData->mode;
    m_edrx_value = edrxData->edrx;
    m_edrx_ptw = edrxData->ptw;
    return alc_error_t::OK;
  }

  lte_lc_lte_mode Modem::GetLteMode(void)
  {
    return m_lte_mode;  
  }

  std::optional<float> Modem::GetErdxValue(void)
  {
    return m_edrx_value;
  }

  std::optional<float> Modem::GetEdrxPtw(void)
  {
    return m_edrx_ptw;
  }

  std::optional<lte_lc_nw_reg_status> Modem::GetRegStatus(void)
  {
    lte_lc_nw_reg_status* status { nullptr };

    int result { lte_lc_nw_reg_status_get(status) };
    if (0 != result)
    {
      LOG_ERR("Unable to get Registration Status. Error: %d.", result);
      // Catch-all.
      return std::nullopt;
    }
    m_reg_status = *status;
    return m_reg_status;
  }

  alc_error_t Modem::SetSystemMode(const lte_lc_system_mode systemMode, const lte_lc_system_mode_preference modePreference)
  {
    int result { lte_lc_system_mode_set(systemMode, modePreference) };
    if (0 != result)
    {
      LOG_ERR("Unable to set System Mode and / or preference. Error: %d.", result);
      if (result == -EINVAL) { return alc_error_t::MODE_OR_PREFERENCE_ERROR; }
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; }
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::GetSystemModeAndPreference(void)
  {
    int result { lte_lc_system_mode_get(&m_system_mode, &m_system_mode_preference) };
    if (0 != result)
    {
      LOG_ERR("Unable to get System Mode and / or preference. Error: %d.", result);
      if (result == -EINVAL) { return alc_error_t::MODE_OR_PREFERENCE_ERROR; }
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; }
      // Catch-all.
      return alc_error_t::FAIL;
    }

    switch (m_system_mode)
    {
      case LTE_LC_SYSTEM_MODE_LTEM:
        LOG_INF("System Mode - LTE-M.");
      break;
      case LTE_LC_SYSTEM_MODE_NBIOT:
        LOG_INF("System Mode - NB-IoT.");
      break;
      case LTE_LC_SYSTEM_MODE_GPS:
        LOG_INF("System Mode - GNSS.");
      break;
      case LTE_LC_SYSTEM_MODE_LTEM_GPS:
        LOG_WRN("System Mode - LTE-M and GPS.");
      break;
      case LTE_LC_SYSTEM_MODE_NBIOT_GPS:
        LOG_INF("System Mode - NB-IoT and GPS.");
      break;
      case LTE_LC_SYSTEM_MODE_LTEM_NBIOT:
        LOG_INF("System Mode - LTE-M and NB-IoT.");
      break;
      case LTE_LC_SYSTEM_MODE_LTEM_NBIOT_GPS:
        LOG_INF("System Mode - LTE-M, NB-IoT and GPS.");
      break;
    }
    
    switch (m_system_mode_preference)
    {
      case LTE_LC_SYSTEM_MODE_PREFER_AUTO:
        LOG_INF("System Mode Preference - Auto.");
      break;
      case LTE_LC_SYSTEM_MODE_PREFER_LTEM:
        LOG_INF("System Mode Preference - LTE-M.");
      break;
      case LTE_LC_SYSTEM_MODE_PREFER_NBIOT:
        LOG_INF("System Mode Preference - NB-IoT.");
      break;
      case LTE_LC_SYSTEM_MODE_PREFER_LTEM_PLMN_PRIO:
        LOG_WRN("System Mode Preference - LTE-M but PLMN has priority.");
      break;
      case LTE_LC_SYSTEM_MODE_PREFER_NBIOT_PLMN_PRIO:
        LOG_INF("System Mode Preference - NB-IoT but PLMN has priority.");
      break;
  
    }

    return alc_error_t::OK;
  }

  alc_error_t Modem::getPsmData(void)
  {
    int* rptau { nullptr };
    int* rat { nullptr };
    //Clear existing data.
    m_psm_rptau = std::nullopt;
    m_psm_rat = std::nullopt;

    int result { lte_lc_psm_get(rptau, rat) };
    if (0 != result)
    {
      LOG_ERR("Unable to get PSM data. Error: %d.", result);
      if (result == -EINVAL) { return alc_error_t::PSM_PARAM_ERROR; }
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; }
      if (result == -EBADMSG) { return alc_error_t::TAU_ERROR; }
      // Catch-all.
      return alc_error_t::FAIL;
    }
    m_psm_rptau = *rptau;
    m_psm_rat = *rat;
    return alc_error_t::OK;
  }

  bool Modem::SetFuncModePowerOff(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_POWER_OFF)) { return false; }
    
    LOG_INF("Function Mode - Power Off.");
    return true;  
  }

  bool Modem::SetFuncModeNormal(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_NORMAL)) { return false; }
  
    LOG_INF("Function Mode - Normal.");
    return true;  
  }

  bool Modem::SetFuncModeRxOnly(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_RX_ONLY)) { return false; }
  
    LOG_INF("Function Mode - RX Only.");
    return true;  
  }

  bool Modem::SetFuncModeOffline(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_OFFLINE)) { return false; }
  
    LOG_INF("Function Mode - Offline.");
    return true;  
  }

  bool Modem::SetFuncModeOffineUiccOn(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_OFFLINE_UICC_ON)) { return false; }
  
    LOG_INF("Function Mode - Offline, UICC On.");
    return true;  
  }

  // bool Modem::SetFuncModeOfflineKeepReg(void)
  // {
  //   if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_OFFLINE_KEEP_REG)) { return false; }
  // 
  //   LOG_INF("Function Mode - Offline Keep Reg.");
  //   return true;  
  // }

  // bool Modem::SetFuncModeOfflineKeepRegUiccOn(void)
  // {
  //   if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_OFFLINE_KEEP_REG_UICC_ON)) { return false; }
  // 
  //   LOG_INF("Function Mode - Offline Keep Reg and UICC on.");
  //   return true;  
  // }

  bool Modem::SetFuncModeDeactivateLte(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_DEACTIVATE_LTE)) { return false; }
  
    LOG_INF("Function Mode - LTE Deactivated.");
    return true;  
  }

  bool Modem::SetFuncModeActivateLte(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_ACTIVATE_LTE)) { return false; }
  
    LOG_INF("Function Mode - LTE Active.");
    return true;  
  }

  bool Modem::SetFuncModeDeactivateGnss(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_DEACTIVATE_GNSS)) { return false; }
  
    LOG_INF("Function Mode - GNSS Deactivated.");
    return true;  
  }

  bool Modem::SetFuncModeActivateGnss(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_ACTIVATE_GNSS)) { return false; }
  
    LOG_INF("Function Mode - GNSS Active.");
    return true;  
  }

  bool Modem::SetFuncModeDeactivateUicc(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_DEACTIVATE_UICC)) { return false; }
  
    LOG_INF("Function Mode - UICC Deactivated.");
    return true;  
  }

  bool Modem::SetFuncModeActivateUicc(void)
  {
    if (alc_error_t::OK != setLteFuncMode(LTE_LC_FUNC_MODE_ACTIVATE_UICC)) { return false; }
  
    LOG_INF("Function Mode - UICC Active.");
    return true;  
  }

  alc_error_t Modem::setLteFuncMode(const lte_lc_func_mode mode)
  {
    int result { lte_lc_func_mode_set(mode) };
    if (0 != result)
    {
      LOG_ERR("Unable to set Modem Function: %d. Error: %d.", mode, result);
      if (result == -EINVAL) { return alc_error_t::FUNC_MODE_PARAM_ERROR; }
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; }
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  std::optional<lte_lc_func_mode> Modem::GetFuncMode(void)
  {
    lte_lc_func_mode* mode { nullptr };
    int result { lte_lc_func_mode_get(mode) };
    if (0 != result)
    {
      LOG_ERR("Unable to get Modem Function. Error: %d.", result);
      return std::nullopt;
    }
    switch (*mode)
    {
      case LTE_LC_FUNC_MODE_POWER_OFF:
        LOG_INF("Function Mode - Power Off.");
        break;
      case LTE_LC_FUNC_MODE_NORMAL:
        LOG_INF("Function Mode - Normal.");
        break;
      case LTE_LC_FUNC_MODE_RX_ONLY:
        LOG_INF("Function Mode - RX Only.");
        break;
      case LTE_LC_FUNC_MODE_OFFLINE:
        LOG_INF("Function Mode - Offline.");
        break;
      case LTE_LC_FUNC_MODE_DEACTIVATE_LTE:
        LOG_INF("Function Mode - LTE Deactivated.");
        break;
      case LTE_LC_FUNC_MODE_ACTIVATE_LTE:
        LOG_INF("Function Mode - LTE Active.");
        break;
      case LTE_LC_FUNC_MODE_DEACTIVATE_GNSS:
        LOG_INF("Function Mode - GNSS Deactiavted.");
        break;
      case LTE_LC_FUNC_MODE_ACTIVATE_GNSS:
        LOG_INF("Function Mode - GNSS Active.");
        break;
      case LTE_LC_FUNC_MODE_DEACTIVATE_UICC:
        LOG_INF("Function Mode - UICC Deactivated.");
        break;
      case LTE_LC_FUNC_MODE_ACTIVATE_UICC:
        LOG_INF("Function Mode - UICC Active.");
        break;
      case LTE_LC_FUNC_MODE_OFFLINE_UICC_ON:
        LOG_INF("Function Mode - OFFLINE, UICC On.");
        break;
      default:
        LOG_WRN("Function Mode - NOT KNOWN.");
        break;
    }
    return *mode;
  }

  alc_error_t Modem::MeasureNeighbourCells(const lte_lc_neighbor_search_type searchType, const uint8_t cellCount)
  {
    // Requires CONFIG_LTE_LC_NEIGHBOR_CELL_MEAS_MODULE.
    // Raises Event LTE_LC_EVT_NEIGHBOR_CELL_MEAS when complete.

    // Cell count range 2 - 15.
    if ((cellCount < 2) || (cellCount > 15)) { return alc_error_t::NCELL_PARAM_ERROR ;}

    lte_lc_ncellmeas_params* params { nullptr };
    params->search_type = searchType;
    params->gci_count = cellCount;

    int result { lte_lc_neighbor_cell_measurement(params) };
    if (0 != result)
    {
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      if (result == -EINVAL) { return alc_error_t::NCELL_PARAM_ERROR; } 
      if (result == -EINPROGRESS) { return alc_error_t::NCELL_MEAS_IN_PROGRESS; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::CancelMeasureNeighbourCells(void)
  {
    int result { lte_lc_neighbor_cell_measurement_cancel() };
    if (0 != result)
    {
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;
  }

  alc_error_t Modem::EnableModemEvents(void)
  {
    int result { lte_lc_modem_events_enable() }; 
    if (0 != result)
    {
      LOG_ERR("Failed to enable Modem Events. Error: %d", result);
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    LOG_INF("Modem Events enabled.");
    return alc_error_t::OK;  
  }

  alc_error_t Modem::DisableModemEvents(void)
  {
    int result { lte_lc_modem_events_disable() }; 
    if (0 != result)
    {
      LOG_ERR("Failed to disable Modem Events. Error: %d", result);
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    LOG_INF("Modem Events disabled.");
    return alc_error_t::OK;  
  }

  alc_error_t Modem::SetPeriodicSearch(void)
  {
    const lte_lc_periodic_search_cfg* config;
    int result { lte_lc_periodic_search_set(config)};
    if (0 != result)
    {
      LOG_ERR("Failed to configure Periodic Search. Error: %d.", result);
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      if (result == -EINVAL) { return alc_error_t::PERIODIC_SEARCH_PARAM_ERROR; } 
      if (result == -EBADMSG) { return alc_error_t::PERIODIC_SEARCH_BAD_RESPONSE; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;  
  }

  alc_error_t Modem::GetPeriodicSearch(void)
  {
    lte_lc_periodic_search_cfg* config;
    int result { lte_lc_periodic_search_get(config)};
    if (0 != result)
    {
      LOG_ERR("Failed to configure Periodic Search. Error: %d.", result);
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      if (result == -ENOENT) { return alc_error_t::PERIODIC_SEARCH_NO_CONFIG; } 
      if (result == -EINVAL) { return alc_error_t::PERIODIC_SEARCH_PARAM_ERROR; } 
      if (result == -EBADMSG) { return alc_error_t::PERIODIC_SEARCH_BAD_RESPONSE; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;  
  }

  alc_error_t Modem::ClearPeriodicSearch(void)
  {
    int result { lte_lc_periodic_search_clear()};
    if (0 != result)
    {
      LOG_ERR("Failed to clear Periodic Search. Error: %d.", result);
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      if (result == -EBADMSG) { return alc_error_t::PERIODIC_SEARCH_BAD_RESPONSE; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;  
  }

  alc_error_t Modem::RequestPeriodicSearch(void)
  {
    int result { lte_lc_periodic_search_request()};
    if (0 != result)
    {
      LOG_ERR("Failed to request Periodic Search. Error: %d.", result);
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      // Catch-all.
      return alc_error_t::FAIL;
    }
    return alc_error_t::OK;  
  }

  alc_error_t Modem::Connect(void)
  {
    int result { lte_lc_connect() };
    if (0 != result)
    {
      if (result == -EFAULT) { return alc_error_t::AT_COMMAND_FAIL; } 
      if (result == -ETIMEDOUT) { return alc_error_t::CONNECTION_TIMEDOUT; } 
      if (result == -EINPROGRESS) { return alc_error_t::CONNECTION_IN_PROGRESS; } 
      // Catch-all.
      return alc_error_t::CONNECT_FAIL;
    }

    return alc_error_t::OK;
  }
}
