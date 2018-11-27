/*
* Add Comment here
*/

#ifndef DRIVESDO_H
#define DRIVESDO_H

#include <mcx/mcx_core.h>
#include "motorcortex_msgs/TorqueControllerCfg.h"
#include "motorcortex_msgs/VelocityControllerCfg.h"
#include "motorcortex_msgs/PositionControllerCfg.h"

class DriveSdo : public mcx::container::Module {
public:

    DriveSdo() = default;

    ~DriveSdo() override = default;

    struct SDOCfg {
        motorcortex_msgs::TorqueControllerCfg torqueControllerCfg;
        motorcortex_msgs::VelocityControllerCfg velocityControllerCfg;
        motorcortex_msgs::PositionControllerCfg positionControllerCfg;
    };

    void setSDOCfg(const SDOCfg& sdoCfg) {
        sdoCfg_ = sdoCfg;
        for (auto& handle : handles_) {
            handle.updateOutputOnce();
        }
        //LOG_INFO("kp: %f", sdoCfg_.velocityControllerCfg.controller_Kp);
        //LOG_INFO("All params updated");
    }

    const SDOCfg &getSDOCfg() const {
        return sdoCfg_;
    }

    void saveAllCfg(const uint32_t& save_cfg) {
        save_cfg_ = save_cfg;
        save_params_handler_.updateOutputOnce();
    }

    void restoreAllCfg(const uint32_t& restore_cfg) {
        restore_cfg_ = restore_cfg;
        restore_params_handler_.updateOutputOnce();
    }

private:
    void create_(const char *name, mcx::parameter_server::Parameter *parameter_server, uint64_t dt_micro_s) override;

    bool initPhase1_() override;

    bool initPhase2_() override;

    bool startOp_() override;

    bool stopOp_() override;

    bool iterateOp_(const mcx::container::TaskTime &system_time, mcx::container::UserTime *user_time) override;

    SDOCfg sdoCfg_;
    std::vector<mcx::parameter_server::ParamHandle> handles_;
    mcx::parameter_server::ParamHandle save_params_handler_;
    mcx::parameter_server::ParamHandle restore_params_handler_;
    uint32_t save_cfg_{};
    uint32_t restore_cfg_{};

};

#endif /* DRIVESDO_H */