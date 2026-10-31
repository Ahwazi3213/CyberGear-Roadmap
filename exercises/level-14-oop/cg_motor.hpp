#ifndef CG_MOTOR_HPP
#define CG_MOTOR_HPP

#include <stdint.h>
extern "C" {
#include "cg_defs.h"
#include "cg_decode.h"
}

/* Abstraksi: "apa pun yang bisa mengirim frame CAN".
 * Di STM32 nanti diisi HalCanTransport; di laptop diisi MockTransport (lihat test). */
class CanTransport {
public:
    virtual ~CanTransport() {}
    virtual bool send(const CgFrame &f) = 0;      // fungsi virtual murni = "kontrak" yang wajib diisi turunan
};

/* Satu objek = satu motor. Semua state motor (ID, feedback terakhir) disembunyikan (private). */
class CyberGearMotor {
public:
    CyberGearMotor(CanTransport &bus, uint8_t hostId, uint8_t motorId);   // constructor berparameter

    /* perintah (mengembalikan hasil send) */
    bool enable();
    bool disable();
    bool clearFault();
    bool setZero();
    bool setRunMode(uint8_t mode);
    bool sendMotion(float pos, float vel, float torque, float kp, float kd);

    /* terima frame: true jika frame ini milik motor ini dan berhasil didecode */
    bool onFrame(const CgFrame &f);

    /* getter (const = tidak mengubah objek) */
    uint8_t id() const;
    float   position() const;
    float   velocity() const;
    float   torque() const;
    float   temperature() const;
    uint8_t mode() const;
    bool    hasFault() const;
    bool    isUpdated() const;
    void    clearUpdated();

private:
    CanTransport &bus_;      // referensi: motor TIDAK memiliki bus, hanya memakainya
    uint8_t       host_;
    uint8_t       id_;
    CgFeedback    fb_;
    bool          updated_;
};

#endif
