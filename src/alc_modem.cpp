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
  static k_sem m_sem_lte_connected { };

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

    if (m_conn_eval_params->mcc == M_MCC_IRELAND) { country = "Ireland"; }

    LOG_INF("Eval: MCC = %d - %s", m_conn_eval_params->mcc, country);

    return m_conn_eval_params->mcc;
  }

  std::optional<int> Modem::GetConnEvalMnc(void)
  {
    if (!validParameters()) { return std::nullopt; }

    const char* netz { "Unknown" };
    if (m_conn_eval_params->mnc == M_MNC_VODAFONE) { netz = "Vodafone"; }
    if (m_conn_eval_params->mnc == M_MNC_O2) { netz = "O2"; }
    if (m_conn_eval_params->mnc == M_MNC_EIR) { netz = "Eir"; }
    if (m_conn_eval_params->mnc == M_MNC_THREE) { netz = "Three"; }

    LOG_INF("Eval: MNC = 0%d - %s", m_conn_eval_params->mnc, netz);

    return m_conn_eval_params->mnc;
  }

  std::optional<uint32_t> Modem::GetConnEvalEutranCellId(void)
  {
    if (!validParameters()) { return std::nullopt; }

    LOG_INF("Eval: Cell Id = %d", m_conn_eval_params->cell_id);

    return m_conn_eval_params->cell_id;
  }

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
    constexpr int count { 0 };
    constexpr int limit { 1 }; 
    k_sem_init(&m_sem_lte_connected, count, limit); 
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
      return alc_error_t::FAIL;;
    }
    m_psm_rptau = *rptau;
    m_psm_rat = *rat;
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
