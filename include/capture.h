// Aurel Strigáč <xstrig00>

#ifndef CAPTURE_H
#define CAPTURE_H

#include <pcap.h>
#include <string>

/**
 * @brief Function for handling separate packets.
 * @param args Argument.
 * @param header Pcap packet header.
 * @param packet Packet data.
 */
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

/**
 * @brief Function for parsing IPv4 packet fields.
 * @param ip_header Pointer to the IPv4 header.
 * @param src_ip Source IP address as a string.
 * @param dst_ip Destination IP address as a string.
 * @param prot_num Protocol number.
 * @param total_len Total length of the packet.
 */
void parse_L3_ipv4(const struct ip *ip_header, std::string &src_ip, std::string &dst_ip, uint16_t &prot_num, uint16_t &total_len);

/**
 * @brief Function for parsing IPv6 packet fields.
 * @param ip6_header Pointer to the IPv6 header.
 * @param src_ip Source IP address as a string.
 * @param dst_ip Destination IP address as a string.
 * @param prot_num Protocol number.
 * @param total_len Total length of the packet.
 */
void parse_L3_ipv6(const struct ip6_hdr *ip6_header, std::string &src_ip, std::string &dst_ip, uint16_t &prot_num, uint16_t &total_len);

/**
 * @brief Function for parsing L4 protocol fields.
 * @param prot_num Protocol number.
 * @param transport_header Pointer to the transport layer header.
 * @param proto_str Protocol name.
 * @param src_port Source port.
 * @param dst_port Destination port.
 */
void parse_L4(uint16_t prot_num, const u_char *transport_header, std::string &proto_str, std::string &src_port, std::string &dst_port);

#endif // CAPTURE_H
