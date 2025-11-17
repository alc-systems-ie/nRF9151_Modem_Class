#pragma once
#include "modem/lte_lc.h"
#include <optional>
#include <string>
#include <zephyr/kernel.h>

namespace alc::modem
{
  enum class alc_error_t
  {
    OK,
    FAIL,
    INIT_FAIL,
    CONNECT_FAIL,
    GET_EVAL_PARAMS_FAIL,
    AT_COMMAND_FAIL,
    CONNECTION_TIMEDOUT,
    CONNECTION_IN_PROGRESS,
    NO_HANDLER,
    OFFLINE_FAIL,
    POWER_OFF_FAIL,
    NORMAL_FAIL,
    PSM_PARAM_ERROR,
    TAU_ERROR

  };

  class Modem
  {
    private:
      constexpr static int M_MCC_IRELAND { 272 };

      constexpr static int M_MNC_VODAFONE { 1 };
      constexpr static int M_MNC_O2 { 2 };
      constexpr static int M_MNC_EIR { 3 };
      constexpr static int M_MNC_THREE { 5 };

      lte_lc_psm_cfg m_psm_config { };
      lte_lc_edrx_cfg m_edrx_config { };
      lte_lc_ncell m_ncell { };
      lte_lc_cell m_cell { };
      lte_lc_cells_info m_cells_info { };
      lte_lc_modem_sleep m_modem_sleep { };
      // lte_lc_invalid_band_conf m_invalid_band_config { };
      lte_lc_modem_evt m_modem_event { };
      lte_lc_rai_cfg m_rai_config { };
      std::optional<lte_lc_conn_eval_params> m_conn_eval_params { };
      lte_lc_ncellmeas_params m_ncellmeas_params { };
      // lte_lc_env_eval_plmn m_env_eval_plmn { };
      // lte_lc_env_eval_params m_env_eval_params { };
      lte_lc_periodic_search_range_cfg m_periodic_search_range_config { };
      lte_lc_periodic_search_table_cfg m_periodic_search_table_config { };
      lte_lc_periodic_search_pattern m_periodic_search_pattern { };
      lte_lc_periodic_search_cfg m_periodic_search_config { };
      // lte_lc_env_eval_result m_env_eval_result { };
      lte_lc_cfun_cb m_cfun_callback { };
      lte_lc_evt m_event { };
      
      // PSM.
      std::optional<int> m_psm_rptau { std::nullopt };
      std::optional<int> m_psm_rat { std::nullopt };

      bool m_conn_eval_param_flag { false }; 

    public:
      Modem();

      alc_error_t InitAndConnect(void);

      alc_error_t RegisterHandler(void);
      alc_error_t DeregisterHandler(void);
      alc_error_t Connect(void);
      alc_error_t ConnectAsync(void);
      alc_error_t SetModemOffline(void);
      alc_error_t SetModemPowerOff(void);
      alc_error_t SetModemNormal(void);
      alc_error_t SetPsmParams(const std::string rpTau, const std::string rat);
      alc_error_t SetPsmParamsSeconds(const int rpTau, const int rat);
      alc_error_t PsmRequest(const bool flag);
      std::optional<int> GetPsmRptau(void);
      std::optional<int> GetPsmRat(void);
      
      // Evaluation parameter results.
      void ShowEvalAll(void);
      std::optional<lte_lc_rrc_mode> GetConnEvalRrcMode(void);
      std::optional<lte_lc_energy_estimate> GetConnEvalEnergyEstimate(void);
      std::optional<lte_lc_tau_triggered> GetConnEvalTauTriggered(void);
      std::optional<lte_lc_ce_level> GetConnEvalCoverageEnhancement(void);
      std::optional<int> GetConnEvalEarfcn(void);
      std::optional<int16_t> GetConnEvalDlPathloss(void);
      std::optional<int16_t> GetConnEvalRsrp(void);
      std::optional<int16_t> GetConnEvalRsrq(void);
      std::optional<int16_t> GetConnEvalTxRepetitions(void);
      std::optional<int16_t> GetConnEvalRxRepetitions(void);
      std::optional<int16_t> GetConnEvalPhysicalCellId(void);
      std::optional<int16_t> GetConnEvalBand(void);
      std::optional<int16_t> GetConnEvalSNR(void);
      std::optional<int16_t> GetConnEvalTxPower(void);
      std::optional<int> GetConnEvalMcc(void);
      std::optional<int> GetConnEvalMnc(void);
      std::optional<uint32_t> GetConnEvalEutranCellId(void);

    private:
      alc_error_t getPsmData(void);
      bool initModem(void);
      bool connectAsync(void);
      static void lteHandler(const struct lte_lc_evt* const evt);
      bool validParameters(void);
      int getConnectionEvaluationParameters(void);
  };
  
}
