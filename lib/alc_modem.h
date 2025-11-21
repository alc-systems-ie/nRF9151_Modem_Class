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
    EDRX_PARAM_ERROR,
    TAU_ERROR,
    EDRX_PARAM_ERRROR,
    PARSE_ERROR,
    REG_STATUS_PARAM_ERROR,
    MODE_OR_PREFERENCE_ERROR,
    FUNC_MODE_PARAM_ERROR,
    NCELL_PARAM_ERROR,
    NCELL_MEAS_IN_PROGRESS,
    ENV_EVAL_MODE_ERROR,
    PERIODIC_SEARCH_PARAM_ERROR,
    PERIODIC_SEARCH_BAD_RESPONSE,
    PERIODIC_SEARCH_NO_CONFIG
  };

  class Modem
  {
    private:
      inline static k_sem m_sem_lte_connected;

      // MCCs.
      constexpr static int M_MCC_IRELAND { 272 };

      constexpr static const char* M_IRELAND { "Ireland" };

      // MNCs.
      constexpr static int M_MNC_VODAFONE { 1 };
      constexpr static int M_MNC_O2 { 2 };
      constexpr static int M_MNC_EIR { 3 };
      constexpr static int M_MNC_THREE { 5 };

      constexpr static const char* M_VODAFONE { "Vodafone" };
      constexpr static const char* M_O2 { "O2" };
      constexpr static const char* M_EIR { "Eir" };
      constexpr static const char* M_THREE { "Three" };

      // Connection Mode.
      lte_lc_lte_mode m_lte_mode { LTE_LC_LTE_MODE_NBIOT };
      std::optional<float> m_edrx_value { std::nullopt };
      std::optional<float> m_edrx_ptw { std::nullopt };

      // Registration.
      inline static std::optional<lte_lc_nw_reg_status> m_reg_status;

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
      lte_lc_psm_cfg m_psm_config { };
      std::optional<int> m_psm_rptau { std::nullopt };
      std::optional<int> m_psm_rat { std::nullopt };

      // Evaluation.
      bool m_conn_eval_param_flag { false }; 

      // System Mode and Preferences.
      lte_lc_system_mode m_system_mode { };
      lte_lc_system_mode_preference m_system_mode_preference { };

    public:
      Modem();

      // Connecton.
      alc_error_t InitAndConnect(void);
      alc_error_t Connect(void);
      alc_error_t ConnectAsync(void);

      // Handler.
      alc_error_t RegisterHandler(void);
      alc_error_t DeregisterHandler(void);

      // Modem.
      alc_error_t SetModemOffline(void);
      alc_error_t SetModemPowerOff(void);
      alc_error_t SetModemNormal(void);
      
      // PSM.
      alc_error_t SetPsmParams(const std::string rpTau, const std::string rat);
      alc_error_t SetPsmParamsSeconds(const int rpTau, const int rat);
      alc_error_t PsmRequest(const bool flag);
      alc_error_t ProprietaryPsmRequest(const bool flag);
      std::optional<int> GetPsmRptau(void);
      std::optional<int> GetPsmRat(void);
      
      // eDrx.
      alc_error_t SetEdrxPtw(const std::string ptw);
      alc_error_t SetEdrxParam(const std::string param);
      alc_error_t EdrxRequest(const bool flag);
      alc_error_t EdrxGet(void);
      std::optional<float> GetErdxValue(void);
      std::optional<float> GetEdrxPtw(void);
      
      // Registration.
      std::optional<lte_lc_nw_reg_status> GetRegStatus(void);

      // System Mode and Preferences.
      alc_error_t SetSystemMode(const lte_lc_system_mode systemMode, const lte_lc_system_mode_preference modePreference);
      alc_error_t GetSystemModeAndPreference(void);

      // Function Modes.
      std::optional<lte_lc_func_mode> GetFuncMode(void);
      bool SetFuncModePowerOff(void);
      bool SetFuncModeNormal(void);
      bool SetFuncModeRxOnly(void);
      bool SetFuncModeOffline(void);
      bool SetFuncModeOffineUiccOn(void);
      bool SetFuncModeOfflineKeepReg(void);
      bool SetFuncModeOfflineKeepRegUiccOn(void);
      bool SetFuncModeDeactivateLte(void);
      bool SetFuncModeActivateLte(void);
      bool SetFuncModeDeactivateGnss(void);
      bool SetFuncModeActivateGnss(void);
      bool SetFuncModeDeactivateUicc(void);
      bool SetFuncModeActivateUicc(void);

      // LTE Mode.
      lte_lc_lte_mode GetLteMode(void);

      // Neighbour Cells.
      alc_error_t MeasureNeighbourCells(const lte_lc_neighbor_search_type searchType, const uint8_t cellCount);
      alc_error_t CancelMeasureNeighbourCells(void);

      // Environment Evaluation. N.B. NOT YET ACTIVE.
      alc_error_t MeasureEnviroment(void);

      // Modem Events.
      alc_error_t EnableModemEvents(void);
      alc_error_t DisableModemEvents(void);

      // Periodic Search.
      alc_error_t SetPeriodicSearch(void);
      alc_error_t GetPeriodicSearch(void);
      alc_error_t ClearPeriodicSearch(void);
      alc_error_t RequestPeriodicSearch(void);


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
      alc_error_t setLteFuncMode(const lte_lc_func_mode mode);
      alc_error_t getPsmData(void);
      bool initModem(void);
      bool connectAsync(void);
      static void lteHandler(const struct lte_lc_evt* const evt);
      bool validParameters(void);
      int getConnectionEvaluationParameters(void);
  };
  
}
