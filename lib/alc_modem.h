#pragma once
#include "modem/lte_lc.h"
#include <optional>
#include <zephyr/kernel.h>

namespace alc::modem
{
  enum class alc_error_t
  {
    OK,
    INIT_FAIL,
    CONNECT_FAIL,
    GET_EVAL_PARAMS_FAIL

  };

  class Modem
  {
    private:
      constexpr static int M_MCC_IRELAND { 272 };


      constexpr static int M_MNC_VODAFONE { 1 };
      constexpr static int M_MNC_O2 { 2 };
      constexpr static int M_MNC_EIR { 3 };
      constexpr static int M_MNC_THREE { 5 };

      std::optional<lte_lc_conn_eval_params> m_conn_eval_params { };
      bool m_conn_eval_param_flag { false }; 


    public:
      Modem();

      alc_error_t Configure(void);
      
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
      static void lteHandler(const struct lte_lc_evt* const evt);
      bool validParameters(void);
      int getConnectionEvaluationParameters(void);
  };
  
}
