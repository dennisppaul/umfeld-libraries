/*
 * GPIO for Linux
 *
 * This file is part of the *GPIO for Linux* library
 * Copyright (c) 2025 Dennis P Paul
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <gpiod.h>
#include <cstdint>
#include <unordered_map>
#include <mutex>

enum PinMode {
    INPUT,
    OUTPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN
};

namespace umfeld {

    constexpr unsigned GPIO_2_SDA1       = 2;
    constexpr unsigned GPIO_3_SCL1       = 3;
    constexpr unsigned GPIO_4            = 4;
    constexpr unsigned GPIO_5            = 5;
    constexpr unsigned GPIO_6            = 6;
    constexpr unsigned GPIO_7_SPI0_CE1_N = 7;
    constexpr unsigned GPIO_8_SPI0_CE0_N = 8;
    constexpr unsigned GPIO_9_SPI0_MISO  = 9;
    constexpr unsigned GPIO_10_SPI0_MOSI = 10;
    constexpr unsigned GPIO_11_SPI0_SCLK = 11;
    constexpr unsigned GPIO_12_PWM0      = 12;
    constexpr unsigned GPIO_13_PWM1      = 13;
    constexpr unsigned GPIO_14_TXD0      = 14;
    constexpr unsigned GPIO_15_RXD0      = 15;
    constexpr unsigned GPIO_16           = 16;
    constexpr unsigned GPIO_17           = 17;
    constexpr unsigned GPIO_18_PWM0      = 18;
    constexpr unsigned GPIO_19_SPI1_MISO = 19;
    constexpr unsigned GPIO_20_SPI1_MOSI = 20;
    constexpr unsigned GPIO_21_SPI1_SCLK = 21;
    constexpr unsigned GPIO_22           = 22;
    constexpr unsigned GPIO_23           = 23;
    constexpr unsigned GPIO_24           = 24;
    constexpr unsigned GPIO_25           = 25;
    constexpr unsigned GPIO_26           = 26;
    constexpr unsigned GPIO_27           = 27;
    constexpr unsigned GPIO_0_ID_SD      = 0;
    constexpr unsigned GPIO_1_ID_SC      = 1;

    class Gpio {
    public:
        static Gpio& instance() {
            static Gpio i;
            return i;
        }

        bool begin(const char* chip_path = "/dev/gpiochip0") {
            std::lock_guard<std::mutex> lock(m_);
            if (chip_) {
                return true;
            }
            chip_ = gpiod_chip_open(chip_path);
            return chip_ != nullptr;
        }

        void end() {
            std::lock_guard<std::mutex> lock(m_);
            for (auto& kv: reqs_) {
                if (kv.second) {
                    gpiod_line_request_release(kv.second);
                }
            }
            reqs_.clear();
            if (chip_) {
                gpiod_chip_close(chip_);
                chip_ = nullptr;
            }
        }

        bool pinMode(uint32_t bcm, PinMode mode) {
            std::lock_guard<std::mutex> lock(m_);
            if (!chip_) {
                return false;
            }

            auto it = reqs_.find(bcm);
            if (it != reqs_.end() && it->second) {
                gpiod_line_request_release(it->second);
                it->second = nullptr;
            }

            gpiod_line_settings* ls = gpiod_line_settings_new();
            if (!ls) {
                return false;
            }

            switch (mode) {
                case OUTPUT:
                    gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_OUTPUT);
                    gpiod_line_settings_set_output_value(ls, GPIOD_LINE_VALUE_INACTIVE);
                    gpiod_line_settings_set_bias(ls, GPIOD_LINE_BIAS_DISABLED);
                    break;
                case INPUT:
                    gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_INPUT);
                    gpiod_line_settings_set_bias(ls, GPIOD_LINE_BIAS_DISABLED);
                    break;
                case INPUT_PULLUP:
                    gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_INPUT);
                    gpiod_line_settings_set_bias(ls, GPIOD_LINE_BIAS_PULL_UP);
                    break;
                case INPUT_PULLDOWN:
                    gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_INPUT);
                    gpiod_line_settings_set_bias(ls, GPIOD_LINE_BIAS_PULL_DOWN);
                    break;
            }

            gpiod_line_config* lc = gpiod_line_config_new();
            if (!lc) {
                gpiod_line_settings_free(ls);
                return false;
            }
            gpiod_line_config_add_line_settings(lc, &bcm, 1, ls);

            gpiod_request_config* rc = gpiod_request_config_new();
            if (!rc) {
                gpiod_line_config_free(lc);
                gpiod_line_settings_free(ls);
                return false;
            }
            gpiod_request_config_set_consumer(rc, "arduino-gpio");

            gpiod_line_request* req = gpiod_chip_request_lines(chip_, rc, lc);

            gpiod_request_config_free(rc);
            gpiod_line_config_free(lc);
            gpiod_line_settings_free(ls);

            if (!req) {
                return false;
            }
            reqs_[bcm] = req;
            return true;
        }

        bool digitalWrite(uint32_t bcm, bool high) {
            std::lock_guard<std::mutex> lock(m_);
            auto                        it = reqs_.find(bcm);
            if (it == reqs_.end() || !it->second) {
                return false;
            }
            return gpiod_line_request_set_value(it->second, bcm,
                                                high ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE) == 0;
        }

        int digitalRead(uint32_t bcm) {
            std::lock_guard<std::mutex> lock(m_);
            auto                        it = reqs_.find(bcm);
            if (it == reqs_.end() || !it->second) {
                return -1;
            }
            int val = gpiod_line_request_get_value(it->second, bcm);
            if (val < 0) {
                return -1;
            }
            return val == GPIOD_LINE_VALUE_ACTIVE ? 1 : 0;
        }

    private:
        Gpio() = default;
        ~Gpio() { end(); }
        Gpio(const Gpio&)            = delete;
        Gpio& operator=(const Gpio&) = delete;

        gpiod_chip*                                       chip_ = nullptr;
        std::unordered_map<uint32_t, gpiod_line_request*> reqs_;
        std::mutex                                        m_;
    };

    inline bool pinMode(uint32_t bcm, PinMode mode) {
        return Gpio::instance().begin() &&
               Gpio::instance().pinMode(bcm, mode);
    }
    inline bool digitalWrite(uint32_t bcm, bool high) {
        return Gpio::instance().digitalWrite(bcm, high);
    }
    inline int digitalRead(uint32_t bcm) {
        return Gpio::instance().digitalRead(bcm);
    }
} // namespace umfeld
