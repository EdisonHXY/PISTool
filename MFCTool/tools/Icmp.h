#ifndef	__ICMP_H__
#define	__ICMP_H__

typedef ULONG IPAddr;       // An IP address.

typedef struct ip_option_information_ICMP
{
    UCHAR   Ttl;                // Time To Live
    UCHAR   Tos;                // Type Of Service
    UCHAR   Flags;              // IP header flags
    UCHAR   OptionsSize;        // Size in bytes of options data
    PUCHAR  OptionsData;        // Pointer to options data
} IP_OPTION_INFORMATION_ICMP, *PIP_OPTION_INFORMATION_ICMP;

typedef struct icmp_echo_reply_ICMP
{
    IPAddr  Address;            // Replying address
    ULONG   Status;             // Reply IP_STATUS
    ULONG   RoundTripTime;      // RTT in milliseconds
    USHORT  DataSize;           // Reply data size in bytes
    USHORT  Reserved;           // Reserved for system use
    PVOID   Data;               // Pointer to the reply data
    struct ip_option_information_ICMP Options; // Reply options
} ICMP_ECHO_REPLY_ICMP, *PICMP_ECHO_REPLY_ICMP;

typedef HANDLE (WINAPI * lpICMP_CreateFile)(void);
typedef BOOL (WINAPI * lpICMP_CloseHandle)(HANDLE hIcmp);
typedef DWORD (WINAPI * lpICMP_SendEcho)(HANDLE IcmpHandle, IPAddr DestinationAddress, 
										 LPVOID RequestData, WORD RequestSize, PIP_OPTION_INFORMATION_ICMP RequestOptions,
										 LPVOID ReplyBuffer, DWORD ReplySize, DWORD Timeout);


#endif