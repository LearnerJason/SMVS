#pragma once

#include <string>
#include <vector>
#include "Dongle_API.h"
#include <ctime>
#pragma comment(lib, "Dongle_s.lib")

using namespace std;

#define ReverseEndianInt(x) ((x) = \
    ((x)&0xff000000) >> 24 |       \
    ((x)&0x00ff0000) >> 8  |       \
    ((x)&0x0000ff00) << 8  |       \
    ((x)&0x000000ff) << 24)

/**
 * NTP Fixed-Point Timestamp Format.
 * From [RFC 5905](http://tools.ietf.org/html/rfc5905).
 */
struct Timestamp
{
    unsigned int seconds;   /**< Seconds since Jan 1, 1900. */
    unsigned int fraction;  /**< Fractional part of seconds. Integer number of 2^-32 seconds. */

    /*
     * Reverses the Endianness of the timestamp.
     * Network byte order is big endian, so it needs to be switched before
     * sending or reading.
     */
    void ReverseEndian(void);

    /**
     * Convert to time_t.
     * Returns the integer part of the timestamp in unix time_t format,
     * which is seconds since Jan 1, 1970.
     */
    time_t to_time_t(void);
};

struct NTPMessage
{
    unsigned char mode :3;           /**< Mode of the message sender. 3 = Client, 4 = Server */
    unsigned char version :2;        /**< Protocol version. Should be set to 3. */
    unsigned char leap :2;           /**< Leap seconds warning. See the [RFC](http://tools.ietf.org/html/rfc5905#section-7.3) */
    unsigned char stratum;          /**< Servers between client and physical timekeeper. 1 = Server is Connected to Physical Source. 0 = Unknown. */
    unsigned char poll;             /**< Max Poll Rate. In log2 seconds. */
    unsigned char precision;        /**< Precision of the clock. In log2 seconds. */
    unsigned int sync_distance;     /**< Round-trip to reference clock. NTP Short Format. */
    unsigned int drift_rate;        /**< Dispersion to reference clock. NTP Short Format. */
    unsigned char ref_clock_id[4];  /**< Reference ID. For Stratum 1 devices, a 4-byte string. For other devices, 4-byte IP address. */
    Timestamp ref;                  /**< Reference Timestamp. The time when the system clock was last updated. */
    Timestamp orig;                 /**< Origin Timestamp. Send time of the request. Copied from the request. */
    Timestamp rx;                   /**< Receive Timestamp. Receipt time of the request. */
    Timestamp tx;                   /**< Transmit Timestamp. Send time of the response. If only a single time is needed, use this one. */

    /*
     * Reverses the Endianness of all the timestamps.
     * Network byte order is big endian, so they need to be switched before
     * sending and after reading.
     *
     * Maintaining them in little endian makes them easier to work with
     * locally, though.
     */
    void ReverseEndian(void);

    /**
     * Recieve an NTPMessage.
     * Overwrites this object with values from the received packet.
     */
    int recv(int sock);
    int sendto(int sock, struct sockaddr_in* srv_addr);
    void clear();
};
//////////////////////////////////////
class CDongleKey
{
public:
	CDongleKey(void);
	~CDongleKey(void);
	// Decrypt the license file and check the configuration information. 
	bool VerifyLicenseFile();
	int  GetAuthorizationType();
	// Make sure the dongle key is plugged in and the authorization is in the key;
	bool VerifyAuthorization();
	void OnSmvsShutdown();
	bool WriteMaxSystemDt();  // Write the maximum system date-time.
	bool TrySyncNetworkTime();
	bool VerifySysDatetime();  // When booted, check if the system date-time are changed manually
	bool CheckExpirationDatetime(bool bWarningMsg = true);
	bool CheckDatetimePeriodically();
	bool GetLockedFlag();
private:
	bool IsSMVSLocked(); // if SMVS is locked now
	void LockSMVS();
	void RemoveLockedFlag();
	bool ParseLicenseFile();
	bool CheckMD5OfEncryptedData();
	
	bool UpdateTotalServiceTime(); // In minute
	bool GetTimeFromServer(SYSTEMTIME& ntpDt); // Get UTC date-time from NTP server

private:
	DONGLE_HANDLE     m_hDongle;
	string            m_strAuthorization; // m_strAuthorization is encrypted string from license file.
	std::vector<BYTE> m_vMD5;         // MD5 value of the plaintext;
	std::vector<BYTE> m_vProductDt;    // Production date and time
	std::vector<BYTE> m_vEncrypedData;// Channel count, authorization type and trail days
	int               m_nChCount;     // Channel Count;
	int               m_AuthType;     // Authorization type - 0:trail; 1:authorised;
	volatile bool     m_bLocked;
	int               m_nTailDays;    // Trail days
	SYSTEMTIME        m_sysLastBootDt;
	SYSTEMTIME        m_sysBootDt;
	SYSTEMTIME        m_maxSysDt;
	long long         m_oldTotalMinutes;
	int               m_nTryCount;   // Try to update the dongle N times,
	DWORD             m_serverTime;
};

