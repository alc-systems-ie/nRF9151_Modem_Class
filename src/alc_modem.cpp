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
        if ((regStatus == LTE_LC_NW_REG_REGISTERED_HOME) || (regStatus == LTE_LC_NW_REG_REGISTERED_ROAMING)) 
        { 
		      LOG_INF("Connection - %s", (regStatus == LTE_LC_NW_REG_REGISTERED_HOME ? "Home Network" : "Roaming"));
     	    k_sem_give(&m_sem_lte_connected);
        }
        break;

      // 1. PSM Update. (Requires CONFIG_LTE_LC_PSM_MODULE).
      case LTE_LC_EVT_PSM_UPDATE:
        LOG_INF("PSM Update.");
        break;

      // 2. eDRX Update. (Requires CONFIG_LTE_LC_EDRX_MODULE).
      case LTE_LC_EVT_EDRX_UPDATE:
        LOG_INF("eDRX Update.");
        break;

      // 3. RRC Status.
	    case LTE_LC_EVT_RRC_UPDATE:
		     LOG_INF("RRC Mode Update: %s", (rrcMode == LTE_LC_RRC_MODE_CONNECTED ? "Connected" : "Idle"));
         (rrcMode == LTE_LC_RRC_MODE_CONNECTED) ? alc::ui::Ui::SetLed2(true) : alc::ui::Ui::SetLed2(false);
		    break;

      // 4. Cell Update.
      case LTE_LC_EVT_CELL_UPDATE:
        LOG_INF("Cell Update.");
        break;

      // 5. Mode Update.
      case LTE_LC_EVT_LTE_MODE_UPDATE:
        LOG_INF("LTE Mode Update.");
        break;

      // 6. Tau Pre-Warning. (Reuires CONFIG_LTE_LC_TAU_PRE_WARNING_MODULE).
      case LTE_LC_EVT_TAU_PRE_WARNING:
        LOG_INF("TAU Pre-warning.");
        break;

      // 7. Neighbour Cell Measurement. (Requires CONFIG_LTE_LC_NEIGHBOR_CELL_MEAS_MODULE).
      case LTE_LC_EVT_NEIGHBOR_CELL_MEAS:
        LOG_INF("Neighbour Cell Measurment.");
        break;

      // 8. Modem Sleep Pre-Warning. (Requires CONFIG_LTE_LC_MODEM_SLEEP_MODULE).
      case LTE_LC_EVT_MODEM_SLEEP_EXIT_PRE_WARNING:
        LOG_INF("Modem Sleep Exit Pre-Warning.");
        break;

      // 9. Modem Sleep Exit. (Requires CONFIG_LTE_LC_MODEM_SLEEP_MODULE).
      case LTE_LC_EVT_MODEM_SLEEP_EXIT:
        LOG_INF("Modem Sleep Exit.");
        break;

      // 10. Modem Sleep Enter. (Requires CONFIG_LTE_LC_MODEM_SLEEP_MODULE).
      case LTE_LC_EVT_MODEM_SLEEP_ENTER:
        LOG_INF("Modem Sleep Enter.");
        break;

      // 11. Modem Event.
      case LTE_LC_EVT_MODEM_EVENT:
        LOG_INF("Modem Event.");
        break;

      // 12. Release Assistance Indication Update. (Requires CONFIG_LTE_LC_RAI_MODULE.)
      case LTE_LC_EVT_RAI_UPDATE:
        LOG_INF("RAI Update.");
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

  alc_error_t Modem::Configure(void)
  {
    constexpr alc_error_t INIT_FAIL { alc_error_t::INIT_FAIL };
    constexpr alc_error_t CONNECT_FAIL { alc_error_t::CONNECT_FAIL };
    constexpr alc_error_t OK { alc_error_t::OK };

	  LOG_INF("Initialising modem library.");
	  int result { nrf_modem_lib_init() };
	  if (result) {
		  LOG_ERR("Failed to initialise the modem library, error: %d.", result);
		  return INIT_FAIL;
	  }
	
	  LOG_INF("Connecting to LTE network");
	  result = lte_lc_connect_async(lteHandler);
	  if (result) {
		  LOG_ERR("Error in lte_lc_connect_async, error: %d", result);
		  return CONNECT_FAIL;
	  }

	  k_sem_take(&m_sem_lte_connected, K_FOREVER);

	  LOG_INF("Connected to LTE network");
    alc::ui::Ui::SetLed1(true);

  	return OK;
  }
}
