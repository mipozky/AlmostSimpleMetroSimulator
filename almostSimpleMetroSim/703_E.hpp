#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <chrono>

#include <vector>
#include <string>

#include "MGraphics.hpp"
#include "mevent.hpp"
#include "train_base.hpp"

#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include "includes.h"

#include <array>
#include <cmath>
#include <functional>

#include <TGui/TGui.hpp>
#include <TGui/Backend/SFML-Graphics.hpp>
#include <map>

class Ent_Train_E : public Ent_Train
{//208 519
 //306 615
protected:
    bool LdoorsOpen = false;
    bool RdoorsOpen = false;
    bool doorsClosed = true;
    void updateuiSprites() {
        ui.switches[0].check.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[0].check.setScale(ui.uiSprites[0].getScale());
        ui.switches[0].NegativeCheck.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[0].NegativeCheck.setScale(ui.uiSprites[0].getScale());
        ui.switches[1].check.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[1].check.setScale(ui.uiSprites[0].getScale());
        ui.switches[1].NegativeCheck.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[1].NegativeCheck.setScale(ui.uiSprites[0].getScale());
        ui.switches[2].check.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[2].check.setScale(ui.uiSprites[0].getScale());
        ui.switches[2].NegativeCheck.setPosition(ui.uiSprites[0].getPosition());
        ui.switches[2].NegativeCheck.setScale(ui.uiSprites[0].getScale());

        ui.gauges[0].setValue(fabs(speed) * 3.6);
        ui.gauges[1].setValue(TrainLine);
        ui.gauges[2].setValue(BrakeLine);

        ui.uiSprites[4].setPosition(ui.levers[0].getPosition());
        ui.uiSprites[4].setScale(ui.levers[0].getScale());


        if (lamps.SD)ui.uiSprites[(int)UISpriteSlot::VUDL].setPosition(ui.uiSprites[(int)UISpriteSlot::Panel].getPosition());
        else ui.uiSprites[(int)UISpriteSlot::VUDL].setPosition(Vector2f(-1000, -1000));
        if (lamps.GRP)ui.uiSprites[(int)UISpriteSlot::GRP].setPosition(ui.uiSprites[(int)UISpriteSlot::Panel].getPosition());
        else ui.uiSprites[(int)UISpriteSlot::GRP].setPosition(Vector2f(-1000, -1000));
        if (lamps.RRP)ui.uiSprites[(int)UISpriteSlot::RRP].setPosition(ui.uiSprites[(int)UISpriteSlot::Panel].getPosition());
        else ui.uiSprites[(int)UISpriteSlot::RRP].setPosition(Vector2f(-1000, -1000));

    }
    struct KV {
        enum class Direction { Backward = -1, Forward = 1, None = 0 };
        int pos = 0;
        bool reverserIn = false;
        Direction reverserPos = Direction::Forward;
        bool RCUreverserIn = false;
        bool RCUreverserON = true;

        bool _F_F7 = false;
        bool _10_14B = false;
        bool _D_D1 = false;
        bool _10_F1 = false;
        bool _U2_4 = false;
        bool _U2_5ZH = false;

        bool _10_8 = false;
        bool _U2_10AK = false;
        bool _U2_2 = false;
        bool _U2_3 = false;
        bool _U2_20 = false;
        bool _U2_25 = false;
        bool _5ZH_5 = false;
        bool _14_14B = false;
        bool _10AK_17 = false;
        bool _U2_6 = false;
        bool _10AK_7A = false;

        void updateContacts() {

            int r = static_cast<int>(reverserPos) + 1;
            static const bool F_F7_tbl[3] = { false, false, true };
            static const bool c10_14B_tbl[3] = { true,  false, true };
            static const bool D_D1_tbl[3] = { true,  false, true };
            static const bool c10_F1_tbl[3] = { true,  true,  false };
            static const bool U2_4_tbl[3] = { true,  false, false };
            static const bool U2_5ZH_tbl[3] = { false, false, true };

            _F_F7 = F_F7_tbl[r];
            _10_14B = c10_14B_tbl[r];
            _D_D1 = D_D1_tbl[r];
            _10_F1 = c10_F1_tbl[r];
            _U2_4 = U2_4_tbl[r];
            _U2_5ZH = U2_5ZH_tbl[r];

            int c = pos+3;
            static const bool c10_8_tbl[7] = { true, false, false, false, false, false, false };
            static const bool U2_10AK_tbl[7] = { true, true,  true,  false, true,  true,  true };
            static const bool U2_2_tbl[7] = { true, true,  false, false, false, true,  true };
            static const bool U2_3_tbl[7] = { true, false, false, false, false, false, true };
            static const bool U2_20_tbl[7] = { true, true,  true,  false, true,  true,  true };
            static const bool U2_25_tbl[7] = { false,true,  false, false, false, false, false };
            static const bool c5ZH_5_tbl[7] = { true, true,  true,  false, false, false, false };
            static const bool c14_14B_tbl[7] = { false,false, false, true,  true,  true,  true };
            static const bool c10AK_17_tbl[7] = { false,false, false, true,  false, false, false };
            static const bool U2_6_tbl[7] = { true, true,  true,  false, false, false, false };
            static const bool c10AK_7A_tbl[7] = { false,false, false, false, true,  true,  true };

            _10_8 = c10_8_tbl[c];
            _U2_10AK = U2_10AK_tbl[c];
            _U2_2 = U2_2_tbl[c];
            _U2_3 = U2_3_tbl[c];
            _U2_20 = U2_20_tbl[c];
            _U2_25 = U2_25_tbl[c];
            _5ZH_5 = c5ZH_5_tbl[c];
            _14_14B = c14_14B_tbl[c];
            _10AK_17 = c10AK_17_tbl[c];
            _U2_6 = U2_6_tbl[c];
            _10AK_7A = c10AK_7A_tbl[c];
        }
    } KV;
    //Ent_Train_E(int id, RenderWindow* window, AssetManager& tm, bool empty, bool isHead = false) : Ent_Train(id, window) {
    //  this->isHead = isHead;
    //
    //}
    double magnetization = 0.0;
    float aux750v = 0.0;
    float power750v = 0.0;

    class EKGController {
    public:
        vector<vector<int>> Configuration;

        bool   WrapsAround = false;
        double RotationRate = 1.0 / 0.12;
        unordered_map<int, double> OverrideRate;

        double Position = 1.0;
        double Velocity = 0.0;
        int    SelectedPosition = 1;
        double MotorState = 0.0;
        double MotorCoilState = 1.0;
        double RKM1 = 0.0;
        double RKM2 = 0.0;
        double RKP = 0.0;

        int MaxPosition = 0;

        vector<double> R;
        vector<double> V;
        vector<double> Contactor;

        virtual void Init() {
            MaxPosition = static_cast<int>(Configuration.size());
            int numContactors = MaxPosition > 0 ? static_cast<int>(Configuration[0].size()) : 0;

            R.assign(numContactors + 1, 1e15);
            V.assign(numContactors + 1, 0.0);
            Contactor.assign(numContactors + 1, 0.0);

            Position = 1.0;
            Velocity = 0.0;
            SelectedPosition = 1;
            MotorState = 0.0;
            MotorCoilState = 1.0;
            RKM1 = RKM2 = RKP = 0.0;
        }

        void TriggerMotorState(double value) {
            if (value > 0.5)       MotorState = 1.0;
            else if (value < -0.5) MotorState = -1.0;
            else                   MotorState = 0.0;
        }
        void TriggerMotorCoilState(double value) {
            MotorCoilState = value;
        }

        virtual void sim(double dt) {
            int position = static_cast<int>(floor(Position + 0.5));
            if (position < 1)            position = 1;
            if (position > MaxPosition)  position = MaxPosition;

            const auto& cfg = Configuration[position - 1];
            for (size_t idx = 0; idx < cfg.size(); ++idx) {
                int k = static_cast<int>(idx) + 1;
                int v = cfg[idx];
                R[k] = 1e-15 + 1e15 * (1 - v);
                V[k] = v;
                Contactor[k] = v;
            }

            double threshold = RotationRate * dt;

            double rate = RotationRate;
            auto it = OverrideRate.find(position);
            if (it != OverrideRate.end()) rate = it->second;

            if (MotorState == 1.0) {
                Velocity = rate * max(-1.0, min(1.0, MotorCoilState));
            }
            if (MotorState == -1.0) {
                Velocity = 0.0;
            }
            Position = Position + min(threshold, Velocity * dt);

            if (!WrapsAround) {
                if (Position > MaxPosition + 0.1) {
                    Position = MaxPosition + 0.1;
                    Velocity = 0.0;
                    MotorState = 0.0;
                }
                if (Position < 0.9) {
                    Position = 0.9;
                    Velocity = 0.0;
                    MotorState = 0.0;
                }
            }
            else {
                if (Position > MaxPosition + 1.0) Position = -1.0;
                if (Position < -1.0)              Position = MaxPosition + 1.0;
            }

            bool BadValues = Position < 0.9 || Position > MaxPosition + 0.1;

            double f = Position - position;
            RKM1 = ((f < -0.30) || (f > 0.30)) ? 1.0 : 0.0;
            RKM2 = (!BadValues && ((f < -0.40) || (f > 0.40))) ? 1.0 : 0.0;
            RKP = ((f > -0.10) && (f < 0.10)) ? 1.0 : 0.0;

            if ((f > -0.40) && (f < 0.40)) SelectedPosition = position;
        }
    };

    class EKG_17A : public EKGController {
    public:
        EKG_17A() {
            Configuration = {
                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
            };
            WrapsAround = true;
            Init();
        }
    };

    class EKG_18A : public EKGController {
    public:
        double PMPos = 0.0;

        function<void(int newSelectedPosition)> OnSelectedPositionChanged;

        EKG_18A() {
            Configuration = {
                {0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                {1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0},
                {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1},
                {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0},
            };
            WrapsAround = true;
            Init();

            RotationRate = 1.0 / 0.30;
        }

        void Init() override {
            EKGController::Init();
            PMPos = 0.0;
            HasOldSelectedPosition = false;
        }

        void sim(double dt) override {
            if (!HasOldSelectedPosition || OldSelectedPosition != SelectedPosition) {
                if (OnSelectedPositionChanged) OnSelectedPositionChanged(SelectedPosition);
                OldSelectedPosition = SelectedPosition;
                HasOldSelectedPosition = true;
            }

            PMPos = ((1.25 < Position && Position < 1.75) ||
                (3.25 < Position && Position < 3.75)) ? 1.0 : 0.0;

            EKGController::sim(dt);
        }

    private:
        int  OldSelectedPosition = 0;
        bool HasOldSelectedPosition = false;
    };
    EKG_17A RheostatController;
    EKG_18A PositionSwitch;
    unordered_map<string, double> KF_50A = {
        {"L25-P37", 0.282},
        {"P35-K2",  0.042},
        {"L26-P31", 0.282},
        {"P29-P28", 0.052},
        {"P30-P29", 0.01125},
        {"P31-P30", 0.01625},
        {"L76-P31", 0.0325},
        {"P35-L18", 0.052},
        {"P36-P35", 0.01125},
        {"P37-P36", 0.01625},
        {"L74-P37", 0.0325}
    };

    unordered_map<string, double> KF_47A = {
    {"L1-L2",   0.84},
    {"L4-L5",   1.46},
    {"L12-L13", 1.730},
    {"P3-P4",   0.13},
    {"P4-P5",   0.224},
    {"P5-P6",   0.184},
    {"P6-P7",   0.224},
    {"P7-P8",   0.224},
    {"P8-P9",   0.183},
    {"P9-P10",  0.13},
    {"P10-P11", 0.136},
    {"P11-P12", 1.004},
    {"P12-P13", 0.57},
    {"P1-P3",   0.79},
    {"P3-P14",  1.622},
    {"P13-P42", 0.285},
    {"P16-P17", 1.15},
    {"P17-P18", 0.13},
    {"P18-P19", 0.224},
    {"P19-P20", 0.184},
    {"P20-P21", 0.224},
    {"P21-P22", 0.224},
    {"P22-P23", 0.184},
    {"P23-P24", 0.13},
    {"P24-P25", 0.13},
    {"P25-P26", 0.79},
    {"P17-P76", 0.244},
    {"P76-P27", 1.710},
    {"P11-P14", 0.59},
    {"P15-P16", 0.31},
    {"P25-P27", 0.59},
    {"L2-L4",   1.140},
    {"L24-L39", 1.000},
    {"L40-L63", 1.000}
    };

    unordered_map<string, double> YAS_44V = {
        {"P13-P33", 51.0},
        {"MK1-MK2", 18.75},
        {"P33-P42", 300.0}
    };

    double R[42] = { 0.0 };

    double P1_3 = 0.0;
    double P3_4 = 0.0;
    double P4_5 = 0.0;
    double P5_6 = 0.0;
    double P6_7 = 0.0;
    double P7_8 = 0.0;
    double P8_9 = 0.0;
    double P9_10 = 0.0;
    double P10_11 = 0.0;
    double P11_12 = 0.0;
    double P12_13 = 0.0;
    double P11_14 = 0.0;

    double P15_16 = 0.0;
    double P16_17 = 0.0;
    double P17_18 = 0.0;
    double P18_19 = 0.0;
    double P19_20 = 0.0;
    double P20_21 = 0.0;
    double P21_22 = 0.0;
    double P22_23 = 0.0;
    double P23_24 = 0.0;
    double P24_25 = 0.0;
    double P25_26 = 0.0;
    double P25_27 = 0.0;

    double L26_P31 = 0.0;
    double P31_P30 = 0.0;
    double P30_P29 = 0.0;
    double P29_P28 = 0.0;

    double L25_P37 = 0.0;
    double P37_P36 = 0.0;
    double P36_P35 = 0.0;
    double P35_K2 = 0.0;

    double P33_P42 = 0.0;
    double P13_P33 = 0.0;
    double P13_P42 = 0.0;

    // Helper method for inverse operation (x^-1)
    inline double inv(double val) const {
        return 1.0 / val;
    }

public:
    void InitResistances_81_703() {
        P12_13 = KF_47A.at("P12-P13");
        P11_12 = KF_47A.at("P11-P12");
        P10_11 = KF_47A.at("P10-P11");
        P9_10 = KF_47A.at("P9-P10");
        P8_9 = KF_47A.at("P8-P9");
        P7_8 = KF_47A.at("P7-P8");
        P6_7 = KF_47A.at("P6-P7");
        P5_6 = KF_47A.at("P5-P6");
        P4_5 = KF_47A.at("P4-P5");
        P3_4 = KF_47A.at("P3-P4");
        P1_3 = KF_47A.at("P1-P3");

        P11_14 = KF_47A.at("P11-P14");

        P25_26 = KF_47A.at("P25-P26");
        P24_25 = KF_47A.at("P24-P25");
        P23_24 = KF_47A.at("P23-P24");
        P22_23 = KF_47A.at("P22-P23");
        P21_22 = KF_47A.at("P21-P22");
        P20_21 = KF_47A.at("P20-P21");
        P19_20 = KF_47A.at("P19-P20");
        P18_19 = KF_47A.at("P18-P19");
        P17_18 = KF_47A.at("P17-P18");
        P16_17 = KF_47A.at("P16-P17");
        P15_16 = KF_47A.at("P15-P16");

        P25_27 = KF_47A.at("P25-P27");

        P29_P28 = KF_50A.at("P29-P28");
        P30_P29 = KF_50A.at("P30-P29");
        P31_P30 = KF_50A.at("P31-P30");
        L26_P31 = KF_50A.at("L26-P31");

        P35_K2 = KF_50A.at("P35-K2");
        P36_P35 = KF_50A.at("P36-P35");
        P37_P36 = KF_50A.at("P37-P36");
        L25_P37 = KF_50A.at("L25-P37");

        P33_P42 = YAS_44V.at("P33-P42");
        P13_P33 = YAS_44V.at("P13-P33");
        P13_P42 = KF_47A.at("P13-P42");
    }

    double R1C1() {
        auto& RK = RheostatController.R;
        auto& T = PositionSwitch.R;

        R[1] = inv(inv(RK[15]) + inv(T[20]));
        R[2] = inv(inv(RK[17]) + inv(P11_12));
        R[3] = inv(inv(RK[19]) + inv(P12_13 + R[2]));
        R[4] = inv(inv(T[22]) + inv(P10_11));
        R[5] = inv(inv(T[1]) + inv(P4_5));
        R[6] = inv(inv(RK[1]) + inv(P1_3));
        R[7] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (RK[9]);
        R[8] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (P7_8);
        R[9] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (P6_7);
        R[10] = inv(inv(RK[11]) + inv(R[9]));
        R[11] = inv(inv(RK[7]) + inv(R[8]));
        R[12] = ((P5_6) * (R[7]) + (P5_6) * (R[11]) + (R[7]) * (R[11])) / (P5_6);
        R[13] = ((P5_6) * (R[7]) + (P5_6) * (R[11]) + (R[7]) * (R[11])) / (R[7]);
        R[14] = ((P5_6) * (R[7]) + (P5_6) * (R[11]) + (R[7]) * (R[11])) / (R[11]);
        R[15] = inv(inv(RK[5]) + inv(R[13]));
        R[16] = inv(inv(R[12]) + inv(R[10]));
        R[17] = ((R[14]) * (R[15]) + (R[14]) * (R[5] + P3_4) + (R[15]) * (R[5] + P3_4)) / (R[14]);
        R[18] = ((R[14]) * (R[15]) + (R[14]) * (R[5] + P3_4) + (R[15]) * (R[5] + P3_4)) / (R[15]);
        R[19] = ((R[14]) * (R[15]) + (R[14]) * (R[5] + P3_4) + (R[15]) * (R[5] + P3_4)) / (R[5] + P3_4);
        R[20] = inv(inv(RK[3]) + inv(R[17]));
        R[21] = inv(inv(R[19]) + inv(R[16]));
        R[22] = ((RK[13]) * (R[4] + P9_10) + (RK[13]) * (P8_9)+(R[4] + P9_10) * (P8_9)) / (RK[13]);
        R[23] = ((RK[13]) * (R[4] + P9_10) + (RK[13]) * (P8_9)+(R[4] + P9_10) * (P8_9)) / (R[4] + P9_10);
        R[24] = ((RK[13]) * (R[4] + P9_10) + (RK[13]) * (P8_9)+(R[4] + P9_10) * (P8_9)) / (P8_9);
        R[25] = inv(inv(R[24]) + inv(R[1]));
        R[26] = inv(inv(R[23]) + inv(R[21]));
        R[27] = ((R[22]) * (R[26]) + (R[22]) * (R[18]) + (R[26]) * (R[18])) / (R[22]);
        R[28] = ((R[22]) * (R[26]) + (R[22]) * (R[18]) + (R[26]) * (R[18])) / (R[26]);
        R[29] = ((R[22]) * (R[26]) + (R[22]) * (R[18]) + (R[26]) * (R[18])) / (R[18]);
        R[30] = inv(inv(R[25]) + inv(R[29]));
        R[31] = inv(inv(R[28]) + inv(R[6] + T[19] + P11_14));
        R[32] = inv(inv(R[27]) + inv(R[20]));
        R[33] = inv(inv(R[30]) + inv(R[31] + R[32]));

        return R[33] + R[3];
    }

    double R1C2() {
        auto& RK = RheostatController.R;
        auto& T = PositionSwitch.R;

        R[1] = inv(inv(RK[15]) + inv(T[20]));
        R[2] = inv(inv(RK[17]) + inv(P11_12));
        R[3] = inv(inv(T[22]) + inv(P10_11));
        R[4] = inv(inv(T[1]) + inv(P4_5));
        R[5] = inv(inv(RK[1]) + inv(P1_3));
        R[6] = inv(inv(R[2]) + inv(RK[19] + P12_13));
        R[7] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (RK[9]);
        R[8] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (P7_8);
        R[9] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (P6_7);
        R[10] = inv(inv(R[9]) + inv(RK[11]));
        R[11] = inv(inv(R[8]) + inv(RK[7]));
        R[12] = ((R[7]) * (R[11]) + (R[7]) * (P5_6)+(R[11]) * (P5_6)) / (R[7]);
        R[13] = ((R[7]) * (R[11]) + (R[7]) * (P5_6)+(R[11]) * (P5_6)) / (R[11]);
        R[14] = ((R[7]) * (R[11]) + (R[7]) * (P5_6)+(R[11]) * (P5_6)) / (P5_6);
        R[15] = inv(inv(R[10]) + inv(R[14]));
        R[16] = inv(inv(RK[5]) + inv(R[12]));
        R[17] = ((RK[13]) * (P8_9)+(RK[13]) * (R[3] + P9_10) + (P8_9) * (R[3] + P9_10)) / (RK[13]);
        R[18] = ((RK[13]) * (P8_9)+(RK[13]) * (R[3] + P9_10) + (P8_9) * (R[3] + P9_10)) / (P8_9);
        R[19] = ((RK[13]) * (P8_9)+(RK[13]) * (R[3] + P9_10) + (P8_9) * (R[3] + P9_10)) / (R[3] + P9_10);
        R[20] = inv(inv(R[1]) + inv(R[18]));
        R[21] = inv(inv(R[15]) + inv(R[19]));
        R[22] = ((R[13]) * (R[17]) + (R[13]) * (R[21]) + (R[17]) * (R[21])) / (R[13]);
        R[23] = ((R[13]) * (R[17]) + (R[13]) * (R[21]) + (R[17]) * (R[21])) / (R[17]);
        R[24] = ((R[13]) * (R[17]) + (R[13]) * (R[21]) + (R[17]) * (R[21])) / (R[21]);
        R[25] = inv(inv(R[16]) + inv(R[23]));
        R[26] = inv(inv(R[22]) + inv(R[20]));
        R[27] = ((R[25]) * (R[4] + P3_4) + (R[25]) * (R[24]) + (R[4] + P3_4) * (R[24])) / (R[25]);
        R[28] = ((R[25]) * (R[4] + P3_4) + (R[25]) * (R[24]) + (R[4] + P3_4) * (R[24])) / (R[4] + P3_4);
        R[29] = ((R[25]) * (R[4] + P3_4) + (R[25]) * (R[24]) + (R[4] + P3_4) * (R[24])) / (R[24]);
        R[30] = inv(inv(R[26]) + inv(R[28]));
        R[31] = inv(inv(R[29]) + inv(RK[3]));
        R[32] = inv(inv(R[27]) + inv(R[5] + T[19] + P11_14));
        R[33] = ((R[30]) * (R[32]) + (R[30]) * (R[6]) + (R[32]) * (R[6])) / (R[30]);
        R[34] = ((R[30]) * (R[32]) + (R[30]) * (R[6]) + (R[32]) * (R[6])) / (R[32]);
        R[35] = ((R[30]) * (R[32]) + (R[30]) * (R[6]) + (R[32]) * (R[6])) / (R[6]);
        R[36] = inv(inv(R[31]) + inv(R[35]));
        R[37] = inv(inv(R[36]) + inv(R[34] + R[33]));

        return R[37];
    }

    double R1C3() {
        auto& RK = RheostatController.R;
        auto& T = PositionSwitch.R;

        R[1] = inv(inv(RK[15]) + inv(T[20]));
        R[2] = inv(inv(RK[17]) + inv(P11_12));
        R[3] = inv(inv(T[22]) + inv(P10_11));
        R[4] = inv(inv(T[1]) + inv(P4_5));
        R[5] = inv(inv(RK[1]) + inv(P1_3));
        R[6] = inv(inv(RK[19] + P12_13) + inv(R[2]));
        R[7] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (RK[9]);
        R[8] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (P7_8);
        R[9] = ((RK[9]) * (P7_8)+(RK[9]) * (P6_7)+(P7_8) * (P6_7)) / (P6_7);
        R[10] = inv(inv(R[9]) + inv(RK[11]));
        R[11] = inv(inv(R[8]) + inv(RK[7]));
        R[12] = ((P5_6) * (R[11]) + (P5_6) * (R[7]) + (R[11]) * (R[7])) / (P5_6);
        R[13] = ((P5_6) * (R[11]) + (P5_6) * (R[7]) + (R[11]) * (R[7])) / (R[11]);
        R[14] = ((P5_6) * (R[11]) + (P5_6) * (R[7]) + (R[11]) * (R[7])) / (R[7]);
        R[15] = inv(inv(R[10]) + inv(R[12]));
        R[16] = inv(inv(R[14]) + inv(RK[5]));
        R[17] = ((RK[13]) * (P9_10 + R[3]) + (RK[13]) * (P8_9)+(P9_10 + R[3]) * (P8_9)) / (RK[13]);
        R[18] = ((RK[13]) * (P9_10 + R[3]) + (RK[13]) * (P8_9)+(P9_10 + R[3]) * (P8_9)) / (P9_10 + R[3]);
        R[19] = ((RK[13]) * (P9_10 + R[3]) + (RK[13]) * (P8_9)+(P9_10 + R[3]) * (P8_9)) / (P8_9);
        R[20] = inv(inv(R[15]) + inv(R[18]));
        R[21] = inv(inv(R[19]) + inv(R[1]));
        R[22] = ((R[20]) * (R[17]) + (R[20]) * (R[13]) + (R[17]) * (R[13])) / (R[20]);
        R[23] = ((R[20]) * (R[17]) + (R[20]) * (R[13]) + (R[17]) * (R[13])) / (R[17]);
        R[24] = ((R[20]) * (R[17]) + (R[20]) * (R[13]) + (R[17]) * (R[13])) / (R[13]);
        R[25] = inv(inv(R[24]) + inv(R[21]));
        R[26] = inv(inv(R[16]) + inv(R[23]));
        R[27] = ((R[22]) * (R[26]) + (R[22]) * (R[4] + P3_4) + (R[26]) * (R[4] + P3_4)) / (R[22]);
        R[28] = ((R[22]) * (R[26]) + (R[22]) * (R[4] + P3_4) + (R[26]) * (R[4] + P3_4)) / (R[26]);
        R[29] = ((R[22]) * (R[26]) + (R[22]) * (R[4] + P3_4) + (R[26]) * (R[4] + P3_4)) / (R[4] + P3_4);
        R[30] = inv(inv(R[25]) + inv(R[29]));
        R[31] = inv(inv(R[27]) + inv(RK[3]));
        R[32] = ((T[19] + R[5]) * (R[31]) + (T[19] + R[5]) * (R[28]) + (R[31]) * (R[28])) / (T[19] + R[5]);
        R[33] = ((T[19] + R[5]) * (R[31]) + (T[19] + R[5]) * (R[28]) + (R[31]) * (R[28])) / (R[31]);
        R[34] = ((T[19] + R[5]) * (R[31]) + (T[19] + R[5]) * (R[28]) + (R[31]) * (R[28])) / (R[28]);
        R[35] = inv(inv(R[30]) + inv(R[32]));
        R[36] = inv(inv(R[33]) + inv(P11_14));
        R[37] = ((R[35]) * (R[36]) + (R[35]) * (R[6]) + (R[36]) * (R[6])) / (R[35]);
        R[38] = ((R[35]) * (R[36]) + (R[35]) * (R[6]) + (R[36]) * (R[6])) / (R[36]);
        R[39] = ((R[35]) * (R[36]) + (R[35]) * (R[6]) + (R[36]) * (R[6])) / (R[6]);
        R[40] = inv(inv(R[39]) + inv(R[34]));
        R[41] = inv(inv(R[40]) + inv(R[37] + R[38]));

        return R[41];
    }

    double R2C1() {
        auto& RK = RheostatController.R;
        auto& T = PositionSwitch.R;

        R[1] = inv(inv(RK[16]) + inv(T[17]));
        R[2] = inv(inv(T[16]) + inv(P24_25));
        R[3] = inv(inv(RK[18]) + inv(P25_26));
        R[4] = inv(inv(T[15]) + inv(P18_19));
        R[5] = inv(inv(RK[2]) + inv(P16_17));
        R[6] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (RK[8]);
        R[7] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (P20_21);
        R[8] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (P19_20);
        R[9] = inv(inv(R[8]) + inv(RK[10]));
        R[10] = inv(inv(R[7]) + inv(RK[6]));
        R[11] = ((R[6]) * (R[10]) + (R[6]) * (R[4] + P17_18) + (R[10]) * (R[4] + P17_18)) / (R[6]);
        R[12] = ((R[6]) * (R[10]) + (R[6]) * (R[4] + P17_18) + (R[10]) * (R[4] + P17_18)) / (R[10]);
        R[13] = ((R[6]) * (R[10]) + (R[6]) * (R[4] + P17_18) + (R[10]) * (R[4] + P17_18)) / (R[4] + P17_18);
        R[14] = inv(inv(R[9]) + inv(R[13]));
        R[15] = inv(inv(RK[4]) + inv(R[11]));
        R[16] = ((R[12]) * (R[5] + P25_27 + P15_16 + RK[20] + T[21]) + (R[12]) * (R[15]) + (R[5] + P25_27 + P15_16 + RK[20] + T[21]) * (R[15])) / (R[12]);
        R[17] = ((R[12]) * (R[5] + P25_27 + P15_16 + RK[20] + T[21]) + (R[12]) * (R[15]) + (R[5] + P25_27 + P15_16 + RK[20] + T[21]) * (R[15])) / (R[5] + P25_27 + P15_16 + RK[20] + T[21]);
        R[18] = ((R[12]) * (R[5] + P25_27 + P15_16 + RK[20] + T[21]) + (R[12]) * (R[15]) + (R[5] + P25_27 + P15_16 + RK[20] + T[21]) * (R[15])) / (R[15]);
        R[19] = inv(inv(R[17]) + inv(R[14]));
        R[20] = inv(inv(R[16]) + inv(R[1]));
        R[21] = ((RK[12]) * (P22_23)+(RK[12]) * (P21_22)+(P22_23) * (P21_22)) / (RK[12]);
        R[22] = ((RK[12]) * (P22_23)+(RK[12]) * (P21_22)+(P22_23) * (P21_22)) / (P22_23);
        R[23] = ((RK[12]) * (P22_23)+(RK[12]) * (P21_22)+(P22_23) * (P21_22)) / (P21_22);
        R[24] = inv(inv(R[19]) + inv(R[22]));
        R[25] = inv(inv(R[23]) + inv(RK[14]));
        R[26] = ((R[18]) * (R[24]) + (R[18]) * (R[21]) + (R[24]) * (R[21])) / (R[18]);
        R[27] = ((R[18]) * (R[24]) + (R[18]) * (R[21]) + (R[24]) * (R[21])) / (R[24]);
        R[28] = ((R[18]) * (R[24]) + (R[18]) * (R[21]) + (R[24]) * (R[21])) / (R[21]);
        R[29] = inv(inv(R[25]) + inv(R[26]));
        R[30] = inv(inv(R[20]) + inv(R[28]));
        R[31] = inv(inv(R[27]) + inv(P23_24 + R[2]));
        R[32] = inv(inv(R[30]) + inv(R[31] + R[29]));

        return R[32] + R[3];
    }

    double R2C2() {
        auto& RK = RheostatController.R;
        auto& T = PositionSwitch.R;

        R[1] = inv(inv(RK[16]) + inv(T[17]));
        R[2] = inv(inv(T[16]) + inv(P24_25));
        R[3] = inv(inv(RK[18]) + inv(P25_26));
        R[4] = inv(inv(T[15]) + inv(P18_19));
        R[5] = inv(inv(RK[2]) + inv(P16_17));
        R[6] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (RK[8]);
        R[7] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (P20_21);
        R[8] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (P19_20);
        R[9] = inv(inv(R[8]) + inv(RK[10]));
        R[10] = inv(inv(R[7]) + inv(RK[6]));
        R[11] = ((R[6]) * (R[10]) + (R[6]) * (R[4] + P17_18) + (R[10]) * (R[4] + P17_18)) / (R[6]);
        R[12] = ((R[6]) * (R[10]) + (R[6]) * (R[4] + P17_18) + (R[10]) * (R[4] + P17_18)) / (R[10]);
        R[13] = ((R[6]) * (R[10]) + (R[6]) * (R[4] + P17_18) + (R[10]) * (R[4] + P17_18)) / (R[4] + P17_18);
        R[14] = inv(inv(R[9]) + inv(R[13]));
        R[15] = inv(inv(RK[4]) + inv(R[11]));
        R[16] = ((R[12]) * (R[14]) + (R[12]) * (P21_22)+(R[14]) * (P21_22)) / (R[12]);
        R[17] = ((R[12]) * (R[14]) + (R[12]) * (P21_22)+(R[14]) * (P21_22)) / (R[14]);
        R[18] = ((R[12]) * (R[14]) + (R[12]) * (P21_22)+(R[14]) * (P21_22)) / (P21_22);
        R[19] = inv(inv(R[18]) + inv(R[15]));
        R[20] = inv(inv(R[16]) + inv(RK[12]));
        R[21] = ((R[17]) * (R[20]) + (R[17]) * (P22_23)+(R[20]) * (P22_23)) / (R[17]);
        R[22] = ((R[17]) * (R[20]) + (R[17]) * (P22_23)+(R[20]) * (P22_23)) / (R[20]);
        R[23] = ((R[17]) * (R[20]) + (R[17]) * (P22_23)+(R[20]) * (P22_23)) / (P22_23);
        R[24] = inv(inv(R[19]) + inv(R[23]));
        R[25] = inv(inv(RK[14]) + inv(R[21]));
        R[26] = ((R[25]) * (R[22]) + (R[25]) * (P23_24 + R[2]) + (R[22]) * (P23_24 + R[2])) / (R[25]);
        R[27] = ((R[25]) * (R[22]) + (R[25]) * (P23_24 + R[2]) + (R[22]) * (P23_24 + R[2])) / (R[22]);
        R[28] = ((R[25]) * (R[22]) + (R[25]) * (P23_24 + R[2]) + (R[22]) * (P23_24 + R[2])) / (P23_24 + R[2]);
        R[29] = inv(inv(R[28]) + inv(R[24]));
        R[30] = inv(inv(R[27]) + inv(R[1]));
        R[31] = inv(inv(R[26]) + inv(R[5] + P25_27 + P15_16 + RK[20] + T[21]));
        R[32] = ((R[30]) * (R[31]) + (R[30]) * (R[3]) + (R[31]) * (R[3])) / (R[30]);
        R[33] = ((R[30]) * (R[31]) + (R[30]) * (R[3]) + (R[31]) * (R[3])) / (R[31]);
        R[34] = ((R[30]) * (R[31]) + (R[30]) * (R[3]) + (R[31]) * (R[3])) / (R[3]);
        R[35] = inv(inv(R[29]) + inv(R[34]));
        R[36] = inv(inv(R[33] + R[32]) + inv(R[35]));

        return R[36];
    }

    double R2C3() {
        auto& RK = RheostatController.R;
        auto& T = PositionSwitch.R;

        R[1] = inv(inv(RK[16]) + inv(T[17]));
        R[2] = inv(inv(T[16]) + inv(P24_25));
        R[3] = inv(inv(RK[18]) + inv(P25_26));
        R[4] = inv(inv(T[15]) + inv(P18_19));
        R[5] = inv(inv(RK[2]) + inv(P16_17));
        R[6] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (RK[8]);
        R[7] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (P20_21);
        R[8] = ((RK[8]) * (P20_21)+(RK[8]) * (P19_20)+(P20_21) * (P19_20)) / (P19_20);
        R[9] = inv(inv(RK[10]) + inv(R[8]));
        R[10] = inv(inv(RK[6]) + inv(R[7]));
        R[11] = ((RK[12]) * (P22_23)+(RK[12]) * (P21_22)+(P22_23) * (P21_22)) / (RK[12]);
        R[12] = ((RK[12]) * (P22_23)+(RK[12]) * (P21_22)+(P22_23) * (P21_22)) / (P22_23);
        R[13] = ((RK[12]) * (P22_23)+(RK[12]) * (P21_22)+(P22_23) * (P21_22)) / (P21_22);
        R[14] = inv(inv(R[9]) + inv(R[12]));
        R[15] = inv(inv(RK[14]) + inv(R[13]));
        R[16] = ((R[6]) * (R[4] + P17_18) + (R[6]) * (R[10]) + (R[4] + P17_18) * (R[10])) / (R[6]);
        R[17] = ((R[6]) * (R[4] + P17_18) + (R[6]) * (R[10]) + (R[4] + P17_18) * (R[10])) / (R[4] + P17_18);
        R[18] = ((R[6]) * (R[4] + P17_18) + (R[6]) * (R[10]) + (R[4] + P17_18) * (R[10])) / (R[10]);
        R[19] = inv(inv(R[16]) + inv(RK[4]));
        R[20] = inv(inv(R[14]) + inv(R[17]));
        R[21] = ((R[18]) * (R[11]) + (R[18]) * (R[20]) + (R[11]) * (R[20])) / (R[18]);
        R[22] = ((R[18]) * (R[11]) + (R[18]) * (R[20]) + (R[11]) * (R[20])) / (R[11]);
        R[23] = ((R[18]) * (R[11]) + (R[18]) * (R[20]) + (R[11]) * (R[20])) / (R[20]);
        R[24] = inv(inv(R[15]) + inv(R[21]));
        R[25] = inv(inv(R[22]) + inv(R[19]));
        R[26] = ((R[5] + P15_16 + RK[20] + T[21]) * (R[23]) + (R[5] + P15_16 + RK[20] + T[21]) * (R[25]) + (R[23]) * (R[25])) / (R[5] + P15_16 + RK[20] + T[21]);
        R[27] = ((R[5] + P15_16 + RK[20] + T[21]) * (R[23]) + (R[5] + P15_16 + RK[20] + T[21]) * (R[25]) + (R[23]) * (R[25])) / (R[23]);
        R[28] = ((R[5] + P15_16 + RK[20] + T[21]) * (R[23]) + (R[5] + P15_16 + RK[20] + T[21]) * (R[25]) + (R[23]) * (R[25])) / (R[25]);
        R[29] = inv(inv(R[24]) + inv(R[26]));
        R[30] = ((P23_24 + R[2]) * (R[29]) + (P23_24 + R[2]) * (R[28]) + (R[29]) * (R[28])) / (P23_24 + R[2]);
        R[31] = ((P23_24 + R[2]) * (R[29]) + (P23_24 + R[2]) * (R[28]) + (R[29]) * (R[28])) / (R[29]);
        R[32] = ((P23_24 + R[2]) * (R[29]) + (P23_24 + R[2]) * (R[28]) + (R[29]) * (R[28])) / (R[28]);
        R[33] = inv(inv(R[31]) + inv(P25_27));
        R[34] = inv(inv(R[32]) + inv(R[1]));
        R[35] = inv(inv(R[27]) + inv(R[30]));
        R[36] = ((R[3]) * (R[34]) + (R[3]) * (R[33]) + (R[34]) * (R[33])) / (R[3]);
        R[37] = ((R[3]) * (R[34]) + (R[3]) * (R[33]) + (R[34]) * (R[33])) / (R[34]);
        R[38] = ((R[3]) * (R[34]) + (R[3]) * (R[33]) + (R[34]) * (R[33])) / (R[33]);
        R[39] = inv(inv(R[36]) + inv(R[37] + R[38]));
        R[40] = inv(inv(R[39]) + inv(R[35]));

        return R[40];
    }

    double S1() {
        const auto& RK = RheostatController.R;

        R[1] = inv(inv(P29_P28) + inv(1e-9 + 1e9 * (1.0 - relays.KSH3.value)));
        R[2] = inv(inv(L26_P31) + inv(RK[21]));
        R[3] = inv(inv(RK[23]) + inv(P31_P30 + R[2]));
        R[4] = inv(inv(P30_P29 + R[3]) + inv(RK[25]));

        return R[4] + R[1];
    }

    double S2() {
        const auto& RK = RheostatController.R;

        R[1] = inv(inv(P35_K2) + inv(1e-9 + 1e9 * (1.0 - relays.KSH4.value)));
        R[2] = inv(inv(L25_P37) + inv(RK[22]));
        R[3] = inv(inv(RK[24]) + inv(P37_P36 + R[2]));
        R[4] = inv(inv(P36_P35 + R[3]) + inv(RK[26]));

        return R[4] + R[1];
    }
    struct Switches {
        bool AV = true;
        bool GV = true;
        bool VU = true;
        bool VU1 = true;
        bool VU2 = true;
        bool VU3 = true;
        bool UAVA = false;

        bool VB = true;
        bool KU1 = true;
        bool KU12 = false;
        bool KU2 = false;
        bool RST = true;

        bool KU3 = true;



        bool KU16 = false;

        bool TLDisconnect = true;
        bool BLDisconnect = true;

        bool EmergencyBrake = false;
        bool EmergencyBrakeValve = false;

        //RST, KU3, VU, >>NORMALY CLOSED
    };
    struct Buttons {
        button KU4;
        button KU5;
        button KU9;
        button KU8;
        button KU11;
        button KU7;
        button KU6;
        button KU10;

        button KU13;

        button UAVAContact;
        void process() {
            KU4.process();
            KU5.process();
            KU9.process();
            KU8.process();
            KU11.process();
            KU7.process();
            KU6.process();
            KU10.process();
            KU13.process();
        }
    } buttons;
    struct Lamps {
        bool GRP;
        bool RRP;
        bool SD;
    } lamps;
    Switches switches;
    struct Relays {
        relay KSH1;
        relay KSH2;
        relay KSH3;
        relay KSH4;
        relay LK1;
        relay LK2;
        relay LK3;
        relay LK4;
        relay TSH;
        relay R1_5;
        relay RV2;
        relay RZ_2;
        relay RV3;
        relay RS;
        relay RPv;
        relay Pneum1;
        relay Pneum2;
        relay RV1;
        relay RRT;
        relay RR;
        relay SR1;
        relay SR2;
        relay RUT;
        relay Rper;
        relay NR;
        relay AVT;
        relay KZ1;
        relay KK;
        relay KO;
        relay BD;
        relay VDZ;
        relay VDOL;
        relay VDOP;
        relay UAVAC{ true };

        relay AK;
        relay AVU;

        LevelRelay RP1_3;
        LevelRelay RP2_4;
        LevelRelay RPL;
        relay RZ_1;
        relay RZ_3;

        void init() {
            LK1.closeTime = 0.1;
            LK1.pneumatic = true;
            LK2.closeTime = 0.1;
            LK2.pneumatic = true;
            LK3.closeTime = 0.1;
            LK3.pneumatic = true;
            LK4.closeTime = 0.1;
            LK4.pneumatic = true;
            RV2.openTime = 1;
            RV1.openTime = 0.7;
            RV3.openTime = 2.3;
            SR1.openTime = 0;
            SR2.openTime = 0.5;
            RP1_3.triggerLevel = 760;
            RP2_4.triggerLevel = 760;
            RPL.triggerLevel = 1400;
            VDOL.closeTime = 0;
            VDOP.closeTime = 0;
        }
        void sim(double dt) {
            KSH1.sim(dt);
            KSH2.sim(dt);
            KSH3.sim(dt);
            KSH4.sim(dt);
            LK1.sim(dt);
            LK2.sim(dt);
            LK3.sim(dt);
            LK4.sim(dt);
            TSH.sim(dt);
            R1_5.sim(dt);
            RV2.sim(dt);
            RZ_2.sim(dt);
            RV3.sim(dt);
            RS.sim(dt);
            RPv.sim(dt);
            Pneum1.sim(dt);
            Pneum2.sim(dt);
            RV1.sim(dt);
            RRT.sim(dt);
            RR.sim(dt);
            SR1.sim(dt);
            SR2.sim(dt);
            RUT.sim(dt);
            Rper.sim(dt);
            NR.sim(dt);
            AVT.sim(dt);
            KZ1.sim(dt);
            KK.sim(dt);
            KO.sim(dt);
            BD.sim(dt);
            VDZ.sim(dt);
            VDOL.sim(dt);
            VDOP.sim(dt);
            AK.sim(dt);
            AVU.sim(dt);
            RP1_3.sim(dt);
            RP2_4.sim(dt);
            RPL.sim(dt);
            RZ_1.sim(dt);
            RZ_3.sim(dt);
			UAVAC.sim(dt);
        }
    } relays;
    double rotationRate = 0.0;
    double magneticState = 0.0;
    double I13 = 0.0;
    double I24 = 0.0;
    double Istator13 = 0.0;
    double Istator24 = 0.0;
    double E13 = 0.0;
    double E24 = 0.0;
    double fieldReduction13 = 0.0;
    double fieldReduction24 = 0.0;
    double bogeyMoment = 0.0;
    float rwa = 0.1819;
    float rws = 0.0396;
    float ranchor = 2 * rwa;
    float rstator = 2 * rws;

    void dk108d_sim(double dt) {
        double currentRotationRate = 3000.0 * (speed * 3.6 / 80.0);
        rotationRate = rotationRate + 5.0 * (currentRotationRate - rotationRate) * dt;

        double a = 0.1204;
        double b = 1.2075;
        double c = 0.3461;

        double rotRate = max(0.0, (rotationRate - 500.0) / 2500.0);
        double rotRateMF = 0.2 + max(0.0, min(1.0, (rotationRate - 375.0) / 1700.0)) * 0.8;

        double Is13 = abs(Istator13);
        double Is24 = abs(Istator24);

        double fluxDenom = 204.7 + (1.0 - min(1.0, rotationRate / 375.0)) * 30.0;

        double flux13 = (Is13 / fluxDenom) * min(1.0, a + b * exp(-c * Is13 / 98.56));
        double flux24 = (Is24 / fluxDenom) * min(1.0, a + b * exp(-c * Is24 / 98.56));

        double MG = (aux750v / 16.0) * magnetization * rotRate;
        if (MG == 0.0) {
            magneticState = magneticState + (MG - magneticState) * dt * 100.0;
        }
        else if (magneticState > MG) {
            magneticState = magneticState - dt * 100.0;
        }
        else {
            magneticState = magneticState + dt * 50.0;
        }

        double magOffset13 = max(0.0, (40.0 + magneticState / 5.0) * 0.007 * (150.0 - Is13) / 150.0);

        flux13 = min(8.0, max(0.01, flux13 + rotRateMF * magnetization * magOffset13));
        flux24 = min(8.0, max(0.01, flux24 + rotRateMF * magnetization * magOffset13));
        constexpr double K_M = 656.0 / (210.0 * 0.716);
        double omega = rotationRate * (M_PI / 30.0);

        E13 = K_M * flux13 * omega;
        E24 = K_M * flux24 * omega;

        E13 = max(-900.0, min(900.0, E13));
        E24 = max(-900.0, min(900.0, E24));


        double absI13 = abs(I13);
        double absI24 = abs(I24);

        double S13 = (I13 > 0.0) ? 1.0 : -1.0;
        double S24 = (I24 > 0.0) ? 1.0 : -1.0;

        double moment13 = S13 * K_M * flux13 * absI13;
        double moment24 = S24 * K_M * flux24 * absI24;

        fieldReduction13 = abs(100.0 * Is13 / (absI13 + 1e-9));
        fieldReduction24 = abs(100.0 * Is24 / (absI24 + 1e-9));

        if (absI13 > 1.0 || absI24 > 1.0) {
            bogeyMoment = (moment13 + moment24) / 2.0;
        }
        else {
            bogeyMoment = 0.0;
        }

        double bogieMotorTorque = moment13 + moment24;
        double bogieAxleTorque = bogieMotorTorque * gearRatio * gearEfficiency;
        double bogieTractionForce = bogieAxleTorque / wheelRadius;

        double muMin = 0.161;
        double muCoeff = 7.5;
        double muOffset = 44.0;
        double speedKmh = speed * 3.6;
        double muV = muMin + muCoeff / (speedKmh + muOffset);

        double axleLoad = (mass / 4.0) * g;
        double maxAdhesionForcePerBogie = muV * axleLoad * 2.0;

        bogieTractionForce = clamp(bogieTractionForce, -maxAdhesionForcePerBogie, maxAdhesionForcePerBogie)/2.3;

        frontBogieTraction = bogieTractionForce;
        rearBogieTraction = bogieTractionForce;
    }
    double gearRatio = 5.33;
    double gearEfficiency = 0.95;
    double wheelRadius = 0.39;
	double frontBogieTraction = 0.0;
	double rearBogieTraction = 0.0;
    float g = 9.81;
    float defMass = 31700;
    double PneumaticPow = 1.2;
    double MaxBrakeDecel = 1.1;
   // int counter = 0;
    void updateMovement(double dt) {
        int dirSign = reverser.VP == 1 ? 1 : -1;

        double tractionForce = (frontBogieTraction + rearBogieTraction) * dirSign;

        double signV = (speed > 0) ? 1.0 : (speed < 0 ? -1.0 : 0.0);
        double BrakeCP = std::pow(std::min(1.0, BrakeCylinderPressure / 2.7), PneumaticPow);

        double brakeRamp = std::max(0.0, std::min(1.0, std::fabs(speed) / 0.3));

        double brakeForce = 0.0;
        if (BrakeCP >= 0.02) {
            brakeForce = -signV * brakeRamp * (mass * MaxBrakeDecel) * BrakeCP;
        }

        bool isLeadingByDrag = (speed > 0) ? (wagonId == 0) : (wagonId == wagonCount - 1);
		bool isLastByDrag = (speed > 0) ? (wagonId == wagonCount - 1) : (wagonId == 0);
        double dragCoeff = isLeadingByDrag ? 5.4 : 0.44;
		dragCoeff += isLastByDrag ? 1.8-0.44 : 0.0;
        double resistanceForce = -signV * (0.001 * mass * g + dragCoeff * speed * speed);

        double gradeForce = -mass * g * std::sin(0);

        netForce = tractionForce + brakeForce + resistanceForce + gradeForce;
       // if (counter == 10) { forceHistory.push_back(netForce); counter = 0; }
       // counter++;
    }
public:
    enum SpriteSlot {
        wheelFF = 0,
        wheelFR = 1,
        wheelRF = 2,
        wheelRR = 3,
        bogeyFront = 4,
        bogeyRear = 5,
        Coupler = 6,
        DoorRL = 0 + 7,
        DoorRR = 1 + 7,
        Salon = 2 + 7,
        Fdoor = 2 + 8,
        DoorLL = 3 + 8,
        DoorLR = 4 + 8,
        Body = 5 + 8,
    };
    enum class UISpriteSlot {
        Panel = 0,
        VUDL = 1,
		RRP = 2,
        GRP = 3,
    };
    enum class GaugeSlot {
        Speed = 0
    };
    enum class ButtonSlot {
        OpenRDoors = 0,
        OpenLDoors = 1,
        OsvV = 3,
        OsvOZ = 4,
        VRP = 5,
        SigN = 6,
        SigD = 7,
        Unkwn = 8,
    };
    enum class SwitchSlot {
        VUD = 0,
    };
    enum class LeverSlot {
        KV = 0,
        KM = 1,
    };

    Ent_Train_E(int id, RenderWindow* window, AssetManager& tm, bool isHead = false, bool reversed = false) : Ent_Train(id, window) {
        this->isHead = isHead;
        using Tx = Texture&;

        wireBus.addSwap(30, 31);
        wireBus.addSwap(3, 4);
        wireBus.swapON = reversed;
		this->reversed = reversed;

        InitResistances_81_703();
        relays.init();

        ui.levers.reserve(8);
        ui.gauges.reserve(8);
        ui.buttons.reserve(16);
        ui.switches.reserve(8);

        TrainLine = 8;
		BrakeLine = 0;
        capacity = 264;

        Tx texBody = tm.get<Texture>("textures\\wagons\\E\\body.png");
        Tx texSalon = tm.get<Texture>("textures\\wagons\\E\\salon.png");
        Tx texDoorLL = tm.get<Texture>("textures\\wagons\\E\\doorsLL.png");
        Tx texDoorLR = tm.get<Texture>("textures\\wagons\\E\\doorsLR.png");
        Tx texDoorRL = tm.get<Texture>("textures\\wagons\\E\\doorsRL.png");
        Tx texDoorRR = tm.get<Texture>("textures\\wagons\\E\\doorsRR.png");
        Tx couplerTex = tm.get<Texture>("textures\\wagons\\E\\scep.png");
        Tx bogeyFront = tm.get<Texture>("textures\\wagons\\E\\bogeyF.png");
        Tx bogeyRear = tm.get<Texture>("textures\\wagons\\E\\bogeyR.png");
        Tx wheelFF = tm.get<Texture>("textures\\wagons\\E\\wheelFF.png");
        Tx wheelFR = tm.get<Texture>("textures\\wagons\\E\\wheelFR.png");
        Tx wheelRF = tm.get<Texture>("textures\\wagons\\E\\wheelRF.png");
        Tx wheelRR = tm.get<Texture>("textures\\wagons\\E\\wheelRR.png");
        Tx Fdoor = tm.get<Texture>("textures\\wagons\\E\\door_front.png");

        auto& spl = spriteList;
        spl.add(Sprite(wheelFF));
        spl.add(Sprite(wheelFR));
        spl.add(Sprite(wheelRF));
        spl.add(Sprite(wheelRR));
        spl.add(Sprite(bogeyFront));
        spl.add(Sprite(bogeyRear));
        spl.add(Sprite(couplerTex));
        spl.add(Sprite(texDoorRL), animDrive(0, -50, 0.4f, animDrive::EaseType::Linear, true));
        spl.add(Sprite(texDoorRR), animDrive(0, 50, 0.4f, animDrive::EaseType::Linear, true));
        spl.add(Sprite(texSalon));
        spl.add(Sprite(Fdoor));
        spl.add(Sprite(texDoorLL), animDrive(0, -50, 0.4f, animDrive::EaseType::Linear, true));
        spl.add(Sprite(texDoorLR), animDrive(0, 50, 0.4f, animDrive::EaseType::Linear, true));
        spl.add(Sprite(texBody));


        using S = SpriteSlot;
        spl.sprites[S::wheelFF].sprite.setOrigin(Vector2f(115, 171));
        spl.sprites[S::wheelFR].sprite.setOrigin(Vector2f(197, 171));
        spl.sprites[S::wheelRF].sprite.setOrigin(Vector2f(596, 171));
        spl.sprites[S::wheelRR].sprite.setOrigin(Vector2f(678, 171));
        spl.sprites[S::wheelFF].relPos = Vector2f(115 * 2, 171 * 2);
        spl.sprites[S::wheelFR].relPos = Vector2f(197 * 2, 171 * 2);
        spl.sprites[S::wheelRF].relPos = Vector2f(596 * 2, 171 * 2);
        spl.sprites[S::wheelRR].relPos = Vector2f(678 * 2, 171 * 2);

        Font& font = tm.get<Font>("fonts/consolas.ttf");


        Texture& texPanel = tm.get<Texture>("textures\\wagons\\E\\interface\\panel.png");
        Texture& texSw3 = tm.get<Texture>("textures\\wagons\\E\\interface\\right_switch_off.png");
        Texture& texSw3on = tm.get<Texture>("textures\\wagons\\E\\interface\\right_switch_on.png");
        Texture& texVUD = tm.get<Texture>("textures\\wagons\\E\\interface\\VUD_light.png");
        Texture& texControllerBase = tm.get<Texture>("textures\\wagons\\E\\interface\\controller_base.png");
        Texture& texControllerHandle = tm.get<Texture>("textures\\wagons\\E\\interface\\controller_handle.png");
        Texture& texControllerTop = tm.get<Texture>("textures\\wagons\\E\\interface\\controller_top.png");
        Texture& speedGauge = tm.get<Texture>("textures\\wagons\\E\\interface\\speed_gauge.png");
        Texture& speedNeedle = tm.get<Texture>("textures\\wagons\\E\\interface\\speed_arrow.png");
        Tx texReversivka = tm.get<Texture>("textures\\wagons\\E\\interface\\reversivka.png");
        Tx texReversivkaRCU = tm.get<Texture>("textures\\wagons\\E\\interface\\reversivkaRCU.png");
        Tx texKranBase = tm.get<Texture>("textures\\wagons\\E\\interface\\kran334_base.png");
        Tx texKranHandle = tm.get<Texture>("textures\\wagons\\E\\interface\\kran334_handle.png");
        Tx BLTLgauge = tm.get<Texture>("textures\\wagons\\E\\interface\\preassure_big.png");
        Tx BLneedle = tm.get<Texture>("textures\\wagons\\E\\interface\\preassure_big_brake.png");
        Tx TLneedle = tm.get<Texture>("textures\\wagons\\E\\interface\\preassure_big_train.png");
        Tx BCPgauge = tm.get<Texture>("textures\\wagons\\E\\interface\\preassure_small.png");
        Tx BCPneedle = tm.get<Texture>("textures\\wagons\\E\\interface\\preassure_small_arrow.png");
        Tx RRPligt = tm.get<Texture>("textures\\wagons\\E\\interface\\RRP_Light.png");
        Tx GRPligt = tm.get<Texture>("textures\\wagons\\E\\interface\\GRP_Light.png");
        Tx ComprSwitchON = tm.get<Texture>("textures\\wagons\\E\\interface\\left_switch_on.png");
        Tx ComprSwitchOFF = tm.get<Texture>("textures\\wagons\\E\\interface\\left_switch_off.png");
        Tx RRKSwitchON = tm.get<Texture>("textures\\wagons\\E\\interface\\middle_switch_on.png");
        Tx RRKSwitchOFF = tm.get<Texture>("textures\\wagons\\E\\interface\\middle_switch_off.png");
        Sprite pSpr(texPanel);
        pSpr.setScale(Vector2f(0.5f, 0.5f));
        pSpr.setPosition(Vector2f(760, 680));
        ui.adduiSprite(pSpr);

        Sprite vudSpr(texVUD);
        vudSpr.setPosition(pSpr.getPosition());
        vudSpr.setScale(pSpr.getScale());
        ui.adduiSprite(vudSpr);

        Sprite rrp(RRPligt);
        rrp.setPosition(pSpr.getPosition());
        rrp.setScale(pSpr.getScale());
        ui.adduiSprite(rrp);

        Sprite grp(GRPligt);
        grp.setPosition(pSpr.getPosition());
        grp.setScale(pSpr.getScale());
        ui.adduiSprite(grp);

        Sprite ControllerTop(texControllerTop);
        ui.adduiSprite(ControllerTop);

        Vector2f pPos = pSpr.getPosition();

        Vector2f leverSize(400, 200);
        Vector2f leverPos(390, 190);
        

        ui.addLever(Vector2f(-150, (window->getSize().y - (593))), leverSize, leverPos, Color::Transparent, font, "", window,
            texControllerHandle, Vector2f(0, 0),
            texControllerBase, Vector2f(0, 0),
            Vector2f(396, 322), -20 + 163 / 2, -163/2, 7,
            []() {}
        );
        ui.addLever(Vector2f(-150, (window->getSize().y - (593))), Vector2f(0, 0), Vector2f(0, 0), Color::Transparent, font, "", window,
            texReversivka, Vector2f(0, 0),
            mg::emptyTex, Vector2f(0, 0),
            Vector2f(396, 322), 180 + 180 - 20, 40, 3,
            []() {}
        );
        ui.addLever(Vector2f(-150, (window->getSize().y - (593))), Vector2f(0, 0), Vector2f(0, 0), Color::Transparent, font, "", window,
            texReversivkaRCU, Vector2f(0, 0),
            mg::emptyTex, Vector2f(0, 0),
            Vector2f(396, 487), 180 + 26, (180 + 155) - (180 + 26), 2,
            []() {}
        );/**/
        //408 441
        ui.addLever(Vector2f(window->getSize().x - 800, (window->getSize().y - (593))), Vector2f(776-331, 441-354), Vector2f(331, 354), Color::Transparent, font, "", window,
            texKranHandle, Vector2f(0, 0),
            texKranBase, Vector2f(0, 0),
            Vector2f(393, 404),67,113, 5,
            [this]() { kran334Pos = 5-ui.levers[3].pos; }
        );
        
        ui.levers[0].startUpdatePos(3);
        ui.levers[1].startUpdatePos(1);
        ui.levers[2].startUpdatePos(1);
        ui.levers[3].startUpdatePos(3); 
        ui.addGauge(Vector2f(300, 300), Vector2f(window->getSize().x - 300, 0), window,
            speedGauge, speedNeedle, Vector2f(0, 0), Vector2f(241, 240),
            0.f, 100.f, 0, 90, 10
        );
        ui.addGauge(Vector2f(300, 300), Vector2f(window->getSize().x - 300, 400), window,
            BLTLgauge, TLneedle, Vector2f(0, 0), Vector2f(149, 234),
            0, 12, -60, 60, 10
        );
        ui.addGauge(Vector2f(300, 300), Vector2f(window->getSize().x - 300, 400), window,
            mg::emptyTex, BLneedle, Vector2f(0, 0), Vector2f(149, 234),
            0, 12, -60, 60, 10
        );
        ui.addButton(
            Vector2f(50, 50),
            Vector2f(pPos.x + 104, pPos.y + 260),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU6.val = true;
            }
        );
        ui.addButton(
            Vector2f(46, 45),
            Vector2f(pPos.x + 335.5f, pPos.y + 197.5f),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU7.val = true;
            }
        );
        ui.addButton(//osveschenie vklucheno
            Vector2f(50, 50),
            Vector2f(pPos.x + 33, pPos.y + 394),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU4.val = true;
            }
        );
        ui.addButton(//osveschenie otkl-zvonok
            Vector2f(50, 50),
            Vector2f(pPos.x + 158, pPos.y + 394),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU5.val = true;
            }
        );
        ui.addButton(//vozvrat rp
            Vector2f(50, 50),
            Vector2f(pPos.x + 283, pPos.y + 394),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU9.val = true;
            }
        );
        ui.addButton(//signalizaciya neispravnosti
            Vector2f(50, 50),
            Vector2f(pPos.x + 416, pPos.y + 394),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU8.val = true;
            }
        );
        ui.addButton(//signalizaciya dverey
            Vector2f(50, 50),
            Vector2f(pPos.x + 543, pPos.y + 394),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU11.val = true;
            }
        );
        ui.addButton(// ???
            Vector2f(50, 50),
            Vector2f(pPos.x + 495, pPos.y + 524),
            Color::Transparent, font, "", window,
            [this]() {
                buttons.KU10.val = true;
            }
        );
        ui.addSwitch(
            Vector2f(49.5f, 46.5f),
            Vector2f(pPos.x + 66 / 2, pPos.y + 586.f / 2),
            font, "VUD", window,
            [this]() {
                switches.KU1 = ui.switches[1].isTicked();
            },
            mg::emptyTex, ComprSwitchON, ComprSwitchOFF
        );
        ui.addSwitch(
            Vector2f(49.5f, 46.5f),
            Vector2f(pPos.x + 317.5f, pPos.y + 294.f),
            font, "VUD", window,
            [this]() {
                switches.KU2 = ui.switches[0].isTicked();
            },
            mg::emptyTex, texSw3on, texSw3
        );
        ui.addSwitch(
            Vector2f(49.5f, 46.5f),
            Vector2f(pPos.x + 354 / 2, pPos.y + 589 / 2),
            font, "VUD", window,
            [this]() {
                switches.KU12 = ui.switches[2].isTicked();
            },
            mg::emptyTex, RRKSwitchON, RRKSwitchOFF
        );
        keyListener.bind(Keyboard::Key::W, [this]() {
            int& pos = KV.pos;
            pos += 1;
            if (pos > 3) pos = 3;
            ui.levers[(int)LeverSlot::KV].startUpdatePos(pos + 3);
            });
        keyListener.bind(Keyboard::Key::S, [this]() {
            int& pos = KV.pos;
            pos -= 1;
            if (pos < -3) pos = -3;
            ui.levers[(int)LeverSlot::KV].startUpdatePos(pos + 3);
            });
        keyListener.bind(Keyboard::Key::Q, [this]() {
            int pos = (int)KV.reverserPos;
            pos -= 1;
            if (pos < -1) pos = -1;
            KV.reverserPos = static_cast<KV::Direction>(pos);
            ui.levers[1].startUpdatePos(pos + 1);
            });
        keyListener.bind(Keyboard::Key::E, [this]() {
            int pos = (int)KV.reverserPos;
            pos += 1;
            if (pos > 1) pos = 1;
            KV.reverserPos = static_cast<KV::Direction>(pos);
            ui.levers[1].startUpdatePos(pos + 1);
            });
        keyListener.bind(Keyboard::Key::Z, [this]() {
            KV.RCUreverserON = true;
            ui.levers[2].startUpdatePos((int)KV.RCUreverserON);
            });
        keyListener.bind(Keyboard::Key::X, [this]() {
            KV.RCUreverserON = false;
            ui.levers[2].startUpdatePos((int)KV.RCUreverserON);
            });
        keyListener.bind(Keyboard::Key::C, [this]() {
            if (KV.RCUreverserIn) return;
            KV.reverserIn = !KV.reverserIn;
            ui.levers[1].handleDrawn = KV.reverserIn;
            });
        keyListener.bind(Keyboard::Key::V, [this]() {
            if (KV.reverserIn) return;
            KV.RCUreverserIn = !KV.RCUreverserIn;
            ui.levers[2].handleDrawn = KV.RCUreverserIn;
            });
        ui.levers[2].handleDrawn = false;
        keyListener.bind(Keyboard::Key::T, [this]() {
            doorsClosed = !doorsClosed;
            if (doorsClosed) { LdoorsOpen = false; RdoorsOpen = false; }
            ui.switches[(int)SwitchSlot::VUD].tick();
            });
        keyListener.bind(Keyboard::Key::A, [this]() {
            if (!doorsClosed) LdoorsOpen = true;
            });
        keyListener.bind(Keyboard::Key::D, [this]() {
            if (!doorsClosed) RdoorsOpen = true;
            });
        keyListener.bind(Keyboard::Key::I, [this]() {
            spriteList.sprites[SpriteSlot::Body].ToggleVisible();
            spriteList.sprites[SpriteSlot::DoorLL].ToggleVisible();
            spriteList.sprites[SpriteSlot::DoorLR].ToggleVisible();
            spriteList.sprites[SpriteSlot::Fdoor].ToggleVisible();
            });

    }
    struct Nodes {
        bool _U2;
        bool _10AK;
        bool _ZR = 1;
        bool _18A;
        bool _4A;
        bool _5A;
        bool _10A;
        bool _10B;
        bool _25A;
        bool _25B;
        bool _10I;
        bool _10T;
        bool _10N;
        bool _10AV;
        bool _10E;
        bool _10AD;
        bool _10AZh;
        bool _10AR;
        bool _10AG;
        bool _10Ya;
        bool _2A;
        bool _2G;
        bool _2E;
        bool _1P;
        bool _6A;
        bool _1G;
        bool _1Zh;
        bool _6K;
        bool _20A;
        bool _20V;
        bool _20G;
        bool _20D;
        bool _23A;
        bool _D1;
        bool _F7;
        bool _11A;

    } nodes;
    struct Reverser {
        bool pos = 0;
        bool VP = 0;
        bool NZ = 1;
        bool VPS = 0;
        bool NZS = 0;
        void sim(double dt) {
            bool newpos = pos;
            if (VPS > 0 and NZS == 0) newpos = 1;
            else if (VPS == 0 and NZS > 0) newpos = 0;
            if (newpos != pos) {
                pos = newpos;
                VP = pos;
                NZ = !pos;
            }
        }
    } reverser;
    array<double, 32> wInput{};
    bool TW18 = 0;
    bool RUTreg = 0;
    bool RUTpod = 0;
    bool RRTpod = 0;
    bool RUTavt = 0;
    bool RRTHold = 0;
    bool AVU = 0;
    bool RedLight = 0;
    bool HeadL = 0;
    bool VUS = 0;
    bool ringing = 0;
    bool eLight1 = 0;
    bool eLight2 = 0;
    bool mLight1 = 0;
    bool mLight2 = 0;
    bool V1;
    float BattaryV = 65;

    void drawBase() override {
        for (auto& s : spriteList.sprites) {
            window->draw(s.sprite);
        }
        //updateRelaysDebug(relays);
    }
    void simElectricAll(double dt) {

        bool BO = fmin(1.0, (double)((BattaryV > 55) * (switches.VB) + wInput[9])); 

        lamps.GRP = BO * relays.RPv.value;
        lamps.RRP = nodes._U2 * wInput[17];
        V1 = BO;
        wireBus.writeWire(9, (BattaryV > 55) * switches.VB);

        nodes._10AK = BO * switches.VU;
        nodes._U2 = nodes._10AK * (KV._U2_10AK + relays.R1_5.value);

        AnnPlaying = wInput[12];
        wireBus.writeWire(13, BO * KV._10_14B * KV._14_14B);

        wireBus.writeWire(3, nodes._U2 * KV._U2_4);
        wireBus.writeWire(4, nodes._U2 * KV._U2_5ZH * (relays.UAVAC.value + KV._5ZH_5));

		//if(isHead)cout << " U2: " << nodes._U2 << " U2_4: " << KV._U2_4 << " U2_5ZH: " << KV._U2_5ZH <<" UAVAC: " << relays.UAVAC.value << " KV._5ZH_5:"<< KV._5ZH_5 << endl;
		//if (isHead) d1 = "U2: " + to_string(nodes._U2) + " U2_4: " + to_string(KV._U2_4) + " U2_5ZH: " + to_string(KV._U2_5ZH) + " UAVAC: " + to_string(relays.UAVAC.value) + " KV._5ZH_5:" + to_string(KV._5ZH_5);
        wireBus.writeWire(0, nodes._10AK * relays.R1_5.value);
        wireBus.writeWire(1, nodes._U2 * KV._U2_2);
        wireBus.writeWire(2, nodes._U2 * KV._U2_3);
        wireBus.writeWire(24, nodes._U2 * KV._U2_25);
        wireBus.writeWire(19, nodes._U2 * KV._U2_20);
        wireBus.writeWire(5, nodes._U2 * KV._U2_6);
        relays.RV2.set(nodes._10AK * KV._10AK_7A);
        relays.R1_5.set(nodes._10AK * relays.RV2.value);
        wireBus.writeWire(16, nodes._10AK * buttons.KU9.value());
        wireBus.writeWire(7, BO * KV._10_8);

        wireBus.writeWire(23, nodes._U2 * buttons.KU8.value());

        relays.RZ_2.set(wireBus.readWire(23) * KV.RCUreverserON * (1 - relays.LK4.value));
        nodes._18A = KV.RCUreverserON * (relays.RPv.value * 100 + (1 - relays.LK4.value));
        wireBus.writeWire(17, nodes._18A);
        TW18 = nodes._18A;

        nodes._4A = wInput[3] * KV.RCUreverserON;
		//d2 = "4A: " + to_string(nodes._4A);
        nodes._5A = wInput[4] * KV.RCUreverserON;
        reverser.NZS = nodes._4A * reverser.VP;
        reverser.VPS = nodes._5A * reverser.NZ;
        relays.LK4.set((nodes._4A * reverser.NZ + nodes._5A * reverser.VP) * !relays.RPv.value * relays.LK3.value * nodes._ZR, TrainLine);
        //cout <<"LK4: " << "(" << nodes._4A << "*" << reverser.NZ << "+" << nodes._5A << "*" << reverser.VP << ") *" << !relays.RPv.value << "*" << relays.LK3.value << "*" << nodes._ZR << endl;

        relays.Pneum1.set(wInput[7] * (PositionSwitch.SelectedPosition == 4 && RheostatController.SelectedPosition >= 1 && RheostatController.SelectedPosition <= 5));
        relays.Pneum2.set(wInput[7] * !relays.RV3.value * !relays.LK4.value);

        relays.RS.set(wInput[11] * KV.RCUreverserON);
        relays.RV3.set(wInput[13] * KV.RCUreverserON);

        nodes._10A = BO * KV.RCUreverserON;

        nodes._10B = nodes._10A * (relays.RV1.value + relays.TSH.value);
        nodes._25B = !relays.TSH.value * relays.LK2.value;
        nodes._25A = relays.KSH2.value + relays.RS.value;

        RUTreg = nodes._10A * (nodes._25B - nodes._25A);

        nodes._10I = nodes._10A * RheostatController.RKM2;

        RUTpod = nodes._10I * relays.LK4.value;
        RRTpod = nodes._10I * (1 - relays.LK2.value);

        if (RRTHold * RRTpod) relays.RRT.close();
        if (1 - RRTHold) relays.RRT.open();
        RRTHold = wInput[24];
        auto RK = floor(RheostatController.SelectedPosition + 0.5);
        auto P = floor(PositionSwitch.SelectedPosition + 0.5);
        float SDRK = 1 - relays.LK4.value * (0.2 + 0.3 * (2 <= RK and RK <= 7 and (P == 1 or P == 3 or P == 4)));

        RheostatController.MotorCoilState = fmin(1.0, nodes._10A * (nodes._10B * relays.RR.value - nodes._10B * !relays.RR.value)) * SDRK;

        nodes._10N = nodes._10A * (RheostatController.RKM1 + relays.SR1.value * (1 - relays.RUT.value));
        nodes._10T = ((1 - relays.SR1.value) + relays.RUT.value) * RheostatController.RKP;
        RheostatController.TriggerMotorState(nodes._10N + nodes._10T * (-10));

        nodes._10AV = nodes._10A * (1 - relays.LK3.value) * (2 <= RK and RK <= 18) * (1 - relays.LK4.value);

        nodes._10E = nodes._10A * ((1 - relays.LK3.value) + relays.Rper.value + PositionSwitch.PMPos);

        relays.SR2.set(nodes._10E * ((P == 3 or P == 4) + relays.KSH2.value) * (1 - relays.LK4.value));

        nodes._10AD = !relays.LK1.value * relays.SR2.value;

        nodes._10AZh = nodes._10AD * relays.TSH.value * (P == 1 or P == 2 or P == 4);

        nodes._10AR = nodes._10AD * (1 - relays.KSH3.value) * (1 - relays.TSH.value) * (2 <= P and P <= 4);
        nodes._10Ya = relays.LK3.value * ((RK == 18) and (P == 1 or P == 3));

        nodes._10AG = nodes._10E * (nodes._10AR + nodes._10Ya + nodes._10AZh);
        PositionSwitch.TriggerMotorState(-1 + 2 * fmax(0, nodes._10AG));

        nodes._2A = wInput[1] * KV.RCUreverserON;
        nodes._2G = nodes._2A * ((P == 1 or P == 3) * (1 <= RK and RK <= 17) + (P == 2 or P == 4) * ((5 <= RK and RK <= 18) + (2 <= RK and RK <= 4) * relays.KSH1.value));
        nodes._2E = nodes._2G * !relays.SR2.value * relays.LK4.value + nodes._10AV;

        relays.RV1.set(nodes._2E * nodes._ZR);
        relays.SR1.set(nodes._2E * !relays.RRT.value * nodes._ZR);
        relays.Rper.set(wInput[2] * KV.RCUreverserON * (17 <= RK and RK <= 18) * nodes._ZR);

        nodes._1P = wInput[0] * KV.RCUreverserON * (P == 1 or P == 2) * relays.NR.value;
        nodes._6A = wInput[5] * KV.RCUreverserON;
        relays.TSH.set(nodes._6A);
        nodes._1G = (nodes._1P + nodes._6A * (P == 3 or P == 4)) * relays.AVT.value * !relays.RPv.value;
        // cout << "1G: (" << nodes._1P << "+"<<nodes._6A << "*"<<(P == 3 or P == 4) << ")*" << relays.AVT.value << "*" << !relays.RPv.value << endl;


        nodes._1Zh = nodes._1G * (relays.LK3.value + relays.KSH2.value * (RK == 1 and (P == 1 or P == 3)));
        relays.LK1.set(nodes._1Zh * (P == 1 or P == 2) * nodes._ZR, TrainLine);
        //cout <<"LK1: " << nodes._1Zh << "*" << (P == 1 or P == 2) << "*" << nodes._ZR << endl;
        relays.LK3.set(nodes._1Zh * nodes._ZR, TrainLine);
        //  cout <<"LK3: " << nodes._1Zh << "*" << nodes._ZR << endl;//cout <<"LK1: " << nodes._1Zh << "*" << (P == 1 or P == 2) << "*" << nodes._ZR << endl;//cout <<"LK2: " << nodes._20A << "*" << relays.LK1.value << "*" << nodes._ZR << endl;//cout <<"LK4: " << "(" << nodes._4A << "*" << reverser.NZ << "+" << nodes._5A << "*" << reverser.VP << ") *" << !relays.RPv.value << "*" << relays.LK3.value << "*" << nodes._ZR << endl;
        relays.RR.set(nodes._1Zh * (P == 1 or P == 3) * nodes._ZR);

        RUTavt = nodes._6A * !relays.KSH2.value;

        nodes._6K = nodes._6A * (RK == 1) * !relays.LK1.value;

        relays.KSH3.set(nodes._6K);
        relays.KSH4.set(nodes._6K);

        if (wInput[16] * KV.RCUreverserON) relays.RPv.open();

        nodes._20A = wInput[19] * KV.RCUreverserON;
        relays.LK2.set(nodes._20A * relays.LK1.value * nodes._ZR, TrainLine);
        //cout <<"LK2: " << nodes._20A << "*" << relays.LK1.value << "*" << nodes._ZR << endl;

        nodes._20V = ((RK == 1 or RK == 18) and P == 1);
        nodes._20G = (1 <= RK and RK <= 5 and (P == 2 or P == 3));

        nodes._20D = nodes._20A * (nodes._20G + nodes._20V * ((1 - relays.Rper.value) + relays.KSH1.value));
        relays.KSH1.set(nodes._20D);
        relays.KSH2.set(nodes._20D);

        wireBus.writeWire(10, BO * switches.VU2);
        nodes._23A = BO * switches.KU1;
        wireBus.writeWire(21, (nodes._23A + wInput[22]) * relays.AK.value);
        wireBus.writeWire(22, nodes._23A);

        AVU = BO * (1 - relays.AVU.value);

        wireBus.writeWire(26, BO * buttons.KU4.value());
        wireBus.writeWire(27, BO * buttons.KU5.value());

        nodes._D1 = BO * KV._D_D1;
        nodes._F7 = BO * KV._F_F7;

        wireBus.writeWire(30, nodes._D1 * (buttons.KU10.value() + buttons.KU6.value() + buttons.KU13.value()));
        wireBus.writeWire(31, nodes._D1 * (buttons.KU10.value() + buttons.KU7.value()));
        wireBus.writeWire(11, nodes._F7 * switches.KU12);

        RedLight = BO * KV._10_F1;

        wireBus.writeWire(15, nodes._D1 * switches.KU2 * switches.KU3);

        HeadL = nodes._F7;
        VUS = nodes._F7 * switches.KU16;

        nodes._11A = wInput[10] * !relays.KZ1.value;
        ringing = nodes._11A + wInput[27];

        eLight1 = BO * switches.VU3 + nodes._11A * !switches.VU3;
        eLight2 = nodes._11A;

        mLight1 = fmax(0, fmin(1.0, (aux750v - 100 - Itotal * 0.25 * (P >= 3) - 25 * relays.KK.value) / 750 * (0.5 + 0.5 * (BattaryV > 55) * switches.VB * relays.KZ1.value)));
        mLight2 = mLight1 * relays.KO.value;

        VPR = BO * switches.RST;
        relays.KK.set(wInput[21]);

        if (wInput[26]) relays.KO.close();
        if (wInput[27]) relays.KO.open();

        bool BD = !relays.BD.value;
        wireBus.writeWire(14, BD * !buttons.KU11.value());
        lamps.SD = (nodes._D1 + BO * buttons.KU11.value()) * (wInput[14] * !buttons.KU11.value() + BD);

        relays.VDZ.set(wInput[15] * BD);
        relays.VDOL.set(wInput[30]);
        relays.VDOP.set(wInput[31]);
    }
    bool VPR;
    bool AnnPlaying;
    void simElectricRK(double dt) {
        bool BO = fmin(1.0, (double)((BattaryV > 55) * (switches.VB) + wInput[9]));
        nodes._10A = BO * KV.RCUreverserON;

        nodes._10B = nodes._10A * (relays.RV1.value + relays.TSH.value);
        nodes._25B = !relays.TSH.value * relays.LK2.value;
        nodes._25A = relays.KSH2.value + relays.RS.value;

        RUTreg = nodes._10A * (nodes._25B - nodes._25A);

        nodes._10I = nodes._10A * RheostatController.RKM2;

        RUTpod = nodes._10I * relays.LK4.value;
        RRTpod = nodes._10I * (1 - relays.LK2.value);

        if (RRTHold * RRTpod) relays.RRT.close();
        if (1 - RRTHold) relays.RRT.open();

        auto RK = floor(RheostatController.SelectedPosition + 0.5);
        auto P = floor(PositionSwitch.SelectedPosition + 0.5);
        float SDRK = 1 - relays.LK4.value * (0.2 + 0.3 * (2 <= RK and RK <= 7 and (P == 1 or P == 3 or P == 4)));

        RheostatController.MotorCoilState = fmin(1.0, nodes._10A * (nodes._10B * relays.RR.value - nodes._10B * !relays.RR.value)) * SDRK;

        nodes._10N = nodes._10A * (RheostatController.RKM1 + relays.SR1.value * (1 - relays.RUT.value));
        nodes._10T = ((1 - relays.SR1.value) + relays.RUT.value) * RheostatController.RKP;
        RheostatController.TriggerMotorState(nodes._10N + nodes._10T * (-10));

        nodes._10AV = nodes._10A * (1 - relays.LK3.value) * (2 <= RK and RK <= 18) * (1 - relays.LK4.value);

        nodes._10E = nodes._10A * ((1 - relays.LK3.value) + relays.Rper.value + PositionSwitch.PMPos);

        relays.SR2.set(nodes._10E * ((P == 3 or P == 4) + relays.KSH2.value) * (1 - relays.LK4.value));

        nodes._10AD = !relays.LK1.value * relays.SR2.value;

        nodes._10AZh = nodes._10AD * relays.TSH.value * (P == 1 or P == 2 or P == 4);

        nodes._10AR = nodes._10AD * (1 - relays.KSH3.value) * (1 - relays.TSH.value) * (2 <= P and P <= 4);
        nodes._10Ya = relays.LK3.value * ((RK == 18) and (P == 1 or P == 3));

        nodes._10AG = nodes._10E * (nodes._10AR + nodes._10Ya + nodes._10AZh);
        PositionSwitch.TriggerMotorState(-1 + 2 * fmax(0, nodes._10AG));

        nodes._2A = wInput[1] * KV.RCUreverserON;
        nodes._2G = nodes._2A * ((P == 1 or P == 3) * (1 <= RK and RK <= 17) + (P == 2 or P == 4) * ((5 <= RK and RK <= 18) + (2 <= RK and RK <= 4) * relays.KSH1.value));
        nodes._2E = nodes._2G * !relays.SR2.value * relays.LK4.value + nodes._10AV;

        relays.RV1.set(nodes._2E * nodes._ZR);
        relays.SR1.set(nodes._2E * !relays.RRT.value * nodes._ZR);
        relays.Rper.set(wInput[2] * KV.RCUreverserON * (17 <= RK and RK <= 18) * nodes._ZR);

        nodes._6A = wInput[5] * KV.RCUreverserON;

        RUTavt = nodes._6A * !relays.KSH2.value;
    }
    void simPS() {
        double CircuitClosed = (power750v * relays.LK1.value > 0) ? 1.0 : 0.0;
        Rtotal = Ranchor13 + Ranchor24 + Rstator13 + Rstator24 + R1 + R2 + R3 + ExtraResistanceLK2;
        Utotal = (power750v - E13 - E24) * relays.LK1.value;
        Itotal = (Utotal / Rtotal) * CircuitClosed;

        I13 = Itotal;
        I24 = Itotal;

        U13 = Utotal * 0.5;
        U24 = Utotal * 0.5;

        R13 = Rtotal;
        R24 = Rtotal;

    }
    double Utotal = 0;
    double Itotal = 0;
    double Rtotal = 0;
    double R13 = 0;
    double R24 = 0;
    void simPP(bool inT) {
        double r1 = Ranchor13 + Rstator13 + R1 + ExtraResistanceLK2;
        double r2 = Ranchor24 + Rstator24 + R2 + ExtraResistanceLK2;
        double r3 = 0.0;

        double v = power750v * relays.LK1.value;
        bool circuitClosed = (v > 0.0);

        if (circuitClosed) {
            double denom = (r1 * r2) + (r1 * r3) + (r2 * r3);

            if (denom != 0.0) {
                I13 = -((E13 * r2 + E13 * r3 - E24 * r3 - r2 * v) / denom);
                I24 = -((E24 * r1 - E13 * r3 + E24 * r3 - r1 * v) / denom);
            }
            else {
                I13 = 0.0;
                I24 = 0.0;
            }
        }
        else {
            I13 = 0.0;
            I24 = 0.0;
        }

        R13 = r1;
        R24 = r2;

        U13 = I13 * r1;
        U24 = I24 * r2;
        Utotal = (U13 + U24) / 2.0;
        Itotal = I13 + I24;
    }
    void simPT() {
        double r1 = Ranchor13 + Rstator13;
        double r2 = Ranchor24 + Rstator24;
        double r3 = R1 + R2 + R3;

        double v = power750v * relays.LK1.value;
        double e1 = E13;
        double e2 = E24;

        double bv = 1;

        double denom = (r1 * r2) + (r1 * r3) + (r2 * r3);

        if (denom != 0.0) {
            I13 = -((e1 * r2 + e1 * r3 - e2 * r3 - r2 * v) / denom) * bv;
            I24 = -((e2 * r1 - e1 * r3 + e2 * r3 - r1 * v) / denom) * bv;
        }
        else {
            I13 = 0.0;
            I24 = 0.0;
        }

        double r_equivalent = 0.0;
        if (r1 > 0.0 && r2 > 0.0) {
            r_equivalent = r3 + inv(inv(r1) + inv(r2));
        }
        else {
            r_equivalent = r3;
        }

        R13 = r_equivalent;
        R24 = r_equivalent;

        U13 = I13 * r1;
        U24 = I24 * r2;
        Utotal = (U13 + U24) / 2.0;
        Itotal = I13 + I24;
    }
    double Rs1;
    double Rs2;
    double Rstator13;
    double Rstator24;
    double Ranchor13;
    double Ranchor24;
    double R1, R2, R3;
    double U13, U24;
    double IR1, IR2, I13SH, I24SH;
    double ExtraResistanceLK2;
    bool SHinit = false;
    float WLR = 1;

    bool compressorON = 0;
    double DoorLinePressure = 0;
    double equalizePressure(double dt, double& pressure, double& pressureDPdT, double target, double rate, double fillRate = DBL_MAX, bool noLimit = false, double smooth = 0.5) {
        if (fillRate != DBL_MAX && target > pressure) rate = fillRate;

        double dPdT = rate;
        if (target < pressure) dPdT = -dPdT;
        double dPdTramp = std::min(1.0, std::fabs(target - pressure) * smooth);
        dPdT *= dPdTramp;

        pressure += dt * dPdT;
        pressure = std::max(0.0, std::min(16.0, pressure));

        pressureDPdT += dPdT;
        if (!noLimit) {
            if (pressure == 0.0)  pressureDPdT = 0.0;
            if (pressure == 16.0) pressureDPdT = 0.0;
        }
        return dPdT;
    }
    double reservoir = 0;
    double TrainLinePressure_dPdT = 0;
    double BrakeLinePressure_dPdT = 0;
    double ReservoirPressure_dPdT = 0;
    double BrakeCylinderPressure_dPdT = 0;
    double TrainToBrakeReducedPressure = 0;
    bool leak = false;
    int kran334Pos = 2;
    double Crane_dPdT = 0;
    double OldBrakeLinePressure = 0;
    bool UAVABlock = 0;
    bool EmergencyValve = 0;
    bool EmergencyValveDisable = 0;
    double EmergencyValve_dPdT = 0;
    double EmergencyBrakeValve_dPdT = 0;
    double BrakeCylinderRegulationError = DBL_MAX;
    double PN1 = 0;
    double PN2 = 0;
    int BePN2 = 0;
    double BrakeCylinderPressure = 0;
    double BCPressure = 0;
    int BrakeCylinderValve = 0;
    bool BrakeEngaged = 0;
    bool TrainLineOverpressureValve = 0;
    void simPneumatic(double dt) {
        TrainLinePressure_dPdT = 0;
        BrakeLinePressure_dPdT = 0;
        ReservoirPressure_dPdT = 0;
        BrakeCylinderPressure_dPdT = 0;
        TrainToBrakeReducedPressure = min(5.1, TrainLine);
        DoorLinePressure = TrainToBrakeReducedPressure * 0.9;
        double trainLineConsumption_dPdT = 0;
        int prs = wagonCount;
        if (leak)prs = prs * 0.3;

        if (kran334Pos == 1) {
            if (switches.TLDisconnect or reservoir > TrainLine) {
                equalizePressure(dt, reservoir, ReservoirPressure_dPdT, TrainLine, 1, DBL_MAX, NULL, 2);
                if (switches.BLDisconnect) equalizePressure(dt, BrakeLine, BrakeLinePressure_dPdT, TrainLine, prs, DBL_MAX, NULL, 2);
            }
        }
        else if (kran334Pos == 2) if (switches.TLDisconnect or reservoir > (TrainToBrakeReducedPressure * 1.05)) equalizePressure(dt, reservoir, ReservoirPressure_dPdT, TrainToBrakeReducedPressure, 0.55, DBL_MAX, NULL, 2);
        else if (kran334Pos == 3) equalizePressure(dt, reservoir, ReservoirPressure_dPdT, 0, 0.001);
        else if (kran334Pos == 4) equalizePressure(dt, reservoir, ReservoirPressure_dPdT, 0, 0.55, DBL_MAX, NULL, 2);
        else if (kran334Pos == 5) {
            equalizePressure(dt, reservoir, ReservoirPressure_dPdT, 0, 1);
            equalizePressure(dt, BrakeLine, BrakeLinePressure_dPdT, 0, prs, DBL_MAX, NULL, 2);
        }

        if (switches.BLDisconnect and (switches.TLDisconnect or reservoir < BrakeLine)) {
            prs = 1.25 * wagonCount;
            if (leak) prs = prs * 0.3;
            equalizePressure(dt, BrakeLine, BrakeLinePressure_dPdT, reservoir, prs, prs * 3) / wagonCount * 2;
        }
        else {
            ReservoirPressure_dPdT = 0;
        }

        trainLineConsumption_dPdT = trainLineConsumption_dPdT + max(0.0, BrakeLinePressure_dPdT);
        trainLineConsumption_dPdT = trainLineConsumption_dPdT + max(0.0, ReservoirPressure_dPdT) * 0.05;

        Crane_dPdT = ReservoirPressure_dPdT;

        leak = 0;
        int Leak = 0;
        OldBrakeLinePressure = BrakeLine;
        if (EmergencyValve) {
            double leakst = 1.1 * wagonCount * clamp(BrakeLine / 4, 0.0, 1.0);
            Leak = equalizePressure(dt, BrakeLine, BrakeLinePressure_dPdT, 0.0, leakst * 2, false, false, 0.4);
            if (Leak >= -0.2 * wagonCount or switches.UAVA != 0) EmergencyValveDisable = true;
            leak = true;
        }
        UAVABlock = BrakeLine > 3.5 and !switches.UAVA;

        EmergencyValve_dPdT = -Leak / wagonCount;

        Leak = 0;

        if (switches.EmergencyBrakeValve) {
            leak = true;
            equalizePressure(dt, BrakeLine, BrakeLinePressure_dPdT, 0.0, (1.1 * wagonCount) * 2, false, false, 0.4);
        }

        EmergencyBrakeValve_dPdT = -Leak / wagonCount;

        if (BrakeCylinderRegulationError == DBL_MAX) BrakeCylinderRegulationError = (static_cast<double>(rand()) / RAND_MAX) * 0.20 - 0.10;

        if (relays.Pneum1.value and !relays.Pneum2.value) {
            if (PN1 == 0) {
                PN1 = min(TrainLine, 1.1 + BrakeCylinderRegulationError + WLR * 0.6);
            }
        }
        else PN1 = 0;
        if (relays.Pneum2.value) {
            if (PN2 == 0) {
                PN2 = min(TrainLine, 2.5 + BrakeCylinderRegulationError + WLR * 1.2);
                //if (!BePN2 and BrakeCylinderPressure > 1.6)
                BePN2 = 1;
            }
        }
        else PN2 = 0;

        double targetPres = max(0.0, min(5.2, 1.5 * (min(5.1, TrainToBrakeReducedPressure) - BrakeLine)));
        if (BCPressure < targetPres) BCPressure = min(targetPres, BCPressure + (0.5 + max(0.0, targetPres - BCPressure - 0.2) * 0.6) * dt);
        else if (BCPressure > targetPres) BCPressure = max(targetPres, BCPressure - 2 * dt);
        double targetPress = PN1 + PN2 + BCPressure;
        if (abs(BrakeCylinderPressure - targetPress) > 0.150) BrakeCylinderValve = 1;
        else if (abs(BrakeCylinderPressure - targetPress) < 0.025) BrakeCylinderValve = 0;

        if (BrakeCylinderValve == 1) {
            equalizePressure(dt, BrakeCylinderPressure, BrakeCylinderPressure_dPdT, min(2.7 + WLR * 1.3, targetPress), 1 + clamp((BrakeCylinderPressure - 0.5) / 2.8, 0.0, 0.7), 3.50, NULL, 0.8 + clamp((BrakeCylinderPressure - 0.75) / 0.6, 0.0, 1.0));
        }
        trainLineConsumption_dPdT = trainLineConsumption_dPdT + max(0.0, BrakeCylinderPressure_dPdT * 0.5);

        if (((BrakeCylinderPressure > 0.2 and BrakeCylinderPressure_dPdT > 0.1) or BrakeCylinderPressure_dPdT > 1) and !BrakeEngaged) {
            BrakeEngaged = true;
        }
        else if (BrakeCylinderPressure < 1 and BrakeCylinderPressure_dPdT < -0.1) BrakeEngaged = false;

        TrainLine = TrainLine - max(0.0, BrakeCylinderPressure_dPdT * 0.002);

        if (!relays.Pneum2.value) {
            if (BePN2 == 1) auto BePN2 = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            else if (BrakeCylinderPressure_dPdT > -0.2 and BePN2 > 0) {
                BePN2 = 0;
            }
        }
        if (BePN2 == 0 and BrakeCylinderPressure_dPdT >= 0.2) BePN2 = -1;
        if ((buttons.UAVAContact.value() > 0.5)  and relays.UAVAC.value < 0.5) {
            relays.UAVAC.set(1);
        }
        relays.UAVAC.set(1);
        compressorON = relays.KK.value * (aux750v > 550);

        TrainLine = TrainLine - 0.07 * trainLineConsumption_dPdT * dt;
        if (compressorON)equalizePressure(dt, TrainLine, TrainLinePressure_dPdT, 10.0, 0.02);
        equalizePressure(dt, TrainLine, TrainLinePressure_dPdT, 0, 0.001);

        if (TrainLine > 9.2) TrainLineOverpressureValve = 1;
        if (TrainLineOverpressureValve == 1) {
            equalizePressure(dt, TrainLine, TrainLinePressure_dPdT, 0.0, 0.2);
            TrainLineOpen = 1;
            if (TrainLine < 5.2) TrainLineOverpressureValve = 0;
        }

        if (BrakeCylinderPressure > 1.9) relays.AVT.open();
        if (BrakeCylinderPressure < 1.2) relays.AVT.close();
        if (TrainLine > 8.2) relays.AK.open();
        if (TrainLine < 6.3) relays.AK.close();

        if (BrakeLine < 2.7) relays.AVU.open();
        if (BrakeLine > 4.3) relays.AVU.close();

        if (DoorLinePressure > 3.5) {
            if (relays.VDOL.value == 1) {
                LdoorsOpen = true;
            }
            if (relays.VDOP.value == 1) {
                RdoorsOpen = true;
            }
            if (relays.VDZ.value) {
                LdoorsOpen = false;
                RdoorsOpen = false;
            }

        }

        relays.BD.set(!RdoorsOpen and !LdoorsOpen);
    }
    void PlaySound(string path) {

    }
	void Autostop() {
		relays.UAVAC.set(0);
	}
    void simElectric2(double dt) {
        if (aux750v > 360) relays.NR.close();
        if (aux750v < 150) relays.NR.open();

        relays.RP1_3.set(fabs(I13));
        relays.RP2_4.set(fabs(I24));
        relays.RPL.set(Itotal);

        double RUTCurrent = (fabs(I13) + fabs(I24)) / 2.0;
        double RUTTarget = 250 + 100 * RUTavt * WLR + 70 * RUTreg;
        if (RUTpod > 0.5)
            relays.RUT.close();
        else
            relays.RUT.set(RUTCurrent > RUTTarget);

        if (relays.RPL.value == 1.0 ||
            relays.RP1_3.value == 1.0 ||
            relays.RP2_4.value == 1.0 ||
            relays.RZ_2.value == 1.0 ||
            relays.RZ_1.value == 1.0 ||
            relays.RZ_3.value == 1.0)
            relays.RPv.close();

        relays.KZ1.set(aux750v > 200);

        
    }
    void simElectric(double dt) {
        power750v = main750v * switches.GV;
        aux750v = power750v * switches.AV;

        bool Brake = relays.TSH.value * relays.LK3.value * relays.LK4.value * (floor(PositionSwitch.SelectedPosition + 0.5) >= 3);

        magnetization = (magnetization + (1 - magnetization) * dt * (0.5 + aux750v / 750 * 1.5)) * Brake;

        ExtraResistanceLK2 = KF_47A["L1-L2"] * (1 - relays.LK2.value) * relays.LK1.value;

        if (floor(PositionSwitch.SelectedPosition + 0.5) == 1) {
            R1 = R1C1();
            R2 = R2C1();
            R3 = 0.0;
        }
        else if (floor(PositionSwitch.SelectedPosition + 0.5) == 2) {
            R1 = R1C2();
            R2 = R2C2();
            R3 = 0.0;
        }
        else if (floor(PositionSwitch.SelectedPosition + 0.5) >= 3) {
            R1 = R1C3();
            R2 = R2C3();
            R3 = 0.0;
        }
        else {
            R1 = 1e9;
            R2 = 1e9;
            R3 = 1e9;
        }
        R1 = R1 + 1e9 * (1 - relays.LK3.value);
        R2 = R2 + 1e9 * (1 - relays.LK4.value);
        Rs1 = S1() + 1e9 * (1 - relays.KSH1.value);
        Rs2 = S2() + 1e9 * (1 - relays.KSH2.value);

        Rstator13 = inv(inv(rstator) + inv(Rs1));
        Rstator24 = inv(inv(rstator) + inv(Rs2));

        Ranchor13 = ranchor;
        Ranchor24 = ranchor;

        if (floor(PositionSwitch.SelectedPosition + 0.5) == 1) {
            simPS();
        }
        else if (floor(PositionSwitch.SelectedPosition + 0.5) == 2) {
            simPP(floor(RheostatController.SelectedPosition + 0.5)  >= 17);
        }
        else {
            simPT();
        }
        IR1 = I13;
        IR2 = I24;

        if (!SHinit) { I13SH = I13; I24SH = I24; SHinit = true; }

        double T13C = fmax(14, fmin(280, R13 * R13 * 2));
        double T24C = fmax(14, fmin(280, R24 * R24 * 2));

        double dI13dT = T13C * (I13 - I13SH) * dt;
        double dI24dT = T24C * (I24 - I24SH) * dt;

        if (dI13dT > 0) dI13dT = fmin(I13 - I13SH, dI13dT);
        if (dI13dT < 0) dI13dT = fmax(I13 - I13SH, dI13dT);
        if (dI24dT > 0) dI24dT = fmin(I24 - I24SH, dI24dT);
        if (dI24dT < 0) dI24dT = fmax(I24 - I24SH, dI24dT);

        I13SH = I13SH + dI13dT;
        I24SH = I24SH + dI24dT;

        I13 = I13SH;
        I24 = I24SH;

        if (floor(PositionSwitch.SelectedPosition + 0.5) == 1) {
            I13 = I13 * relays.LK3.value * relays.LK4.value * relays.LK1.value;
            I24 = I24 * relays.LK3.value * relays.LK4.value * relays.LK1.value;

            I24 = (I24 + I13) * 0.5;
            I13 = I24;
            Itotal = I24;
        }
        else if (floor(PositionSwitch.SelectedPosition + 0.5) == 2) {
            I13 = I13 * relays.LK3.value * relays.LK4.value * relays.LK1.value;
            I24 = I24 * relays.LK3.value * relays.LK4.value * relays.LK1.value;

            Itotal = I13 + I24;
        }
        else {
            I13 = I13 * relays.LK3.value * relays.LK4.value;
            I24 = I24 * relays.LK3.value * relays.LK4.value;

            Itotal = I13 + I24;
        }

        Uanchor13 = I13 * Ranchor13;
        Uanchor24 = I24 * Ranchor24;
        Ustator13 = I13 * Rstator13;
        Ustator24 = I24 * Rstator24;

        Istator13 = Ustator13 / rstator;
        Istator24 = Ustator24 / rstator;

        Ishunt13 = Ustator13 / Rs1;
        Ishunt24 = Ustator24 / Rs2;

        if (floor(PositionSwitch.SelectedPosition + 0.5) >= 3) {
            double origShunt13 = Ishunt13;
            double origStator13 = Istator13;

            Ishunt13 = -Ishunt24;
            Ishunt24 = -origShunt13;

            Istator13 = -Istator24;
            Istator24 = -origStator13;
        }

        if (floor(PositionSwitch.SelectedPosition + 0.5) >= 3) IRT2 = fabs(Itotal);
        else IRT2 = 0;

        if (R1 > 1e5) IR1 = 0;
        if (R2 > 1e5) IR2 = 0;


        for (int i = 0; i < 16; i++)
        {
            for (int J = 0; J < (int)wInput.size(); ++J) {
                wInput[J] = wireBus.readWire(J);
            }
            if (i == 0)simElectricAll(dt/16.0);
            else simElectricRK(dt/16.0);
        }
    }
    double Uanchor13 = 0;
    double Uanchor24 = 0;
    double Ustator13 = 0;
    double Ustator24 = 0;
    double Ishunt13 = 0;
    double Ishunt24 = 0;
    double IRT2;
    int PassCount = 0;
    vector<MEvent> simulate(vector<MEvent>* input, float dt) override {
        if (isHead && input) keyListener.process(*input);

        mass = defMass + PassCount * 70;

        auto& spl = spriteList;
        //auto& W = WireIndex;
        //cout<<KS
        KV.updateContacts();
        simElectric(dt);
        simElectric2(dt);
        simPneumatic(dt);
        reverser.sim(dt);
        relays.sim(dt);

        RheostatController.sim(dt);
        PositionSwitch.sim(dt);
        for(int i = 0; i < 15; i++) {
            dk108d_sim(dt/15);
        }
        updateMovement(dt);
        //cout << relays.LK1.value << endl;
        WLR = max(0.0, min(1.0, PassCount / 200.0));

        //wheel diam 171-153 
        //wheel1 center 171 679
        //wheel2 center 171 597
        //wheel3 center 171 198
        //wheel3 center 171 116

        buttons.process();

        using S = SpriteSlot;

        rotationRate = (speed * METER_TO_PX * 180.0) / (M_PI * 171.0 - 153.0);

        double deltaAngle = rotationRate * dt;
        spl.sprites[S::wheelFF].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelFF].sprite.getRotation());
        spl.sprites[S::wheelFR].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelFR].sprite.getRotation());
        spl.sprites[S::wheelRF].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelRF].sprite.getRotation());
        spl.sprites[S::wheelRR].sprite.setRotation(-degrees(deltaAngle) + spl.sprites[S::wheelRR].sprite.getRotation());

        using U = UISpriteSlot;
        if (isHead) {
            updateuiSprites();
            if (KV.reverserPos == KV::Direction::None) {
                ui.levers[1].pos = 1;
            }
             KV.pos = ui.levers[0].pos - 3;
            KV.reverserPos = static_cast<KV::Direction>(ui.levers[1].pos - 1);
            KV.RCUreverserON = ui.levers[2].pos != 0;

        }

        spriteList.sprites[S::DoorRL].updateAnim(dt, RdoorsOpen);
        spriteList.sprites[S::DoorRR].updateAnim(dt, RdoorsOpen);
        spriteList.sprites[S::DoorLL].updateAnim(dt, LdoorsOpen);
        spriteList.sprites[S::DoorLR].updateAnim(dt, LdoorsOpen);


        spl.updatePositions(pos);

        Ent_Train::simulate(input, dt);

        vector<MEvent> res{};
        return res;
    }
};