// Aurel Strigáč <xstrig00>

#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <cstring>

#include "capture.h"
#include "flow.h"
#include "isa-top.h"

/**
 * @brief Function for handling separate packets.
 * @param args Argument.
 * @param header Pcap packet header.
 * @param packet Pointer to the packet data.
 */
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    (void) args, (void) header;  // Pity fix for unused variable

    const struct ether_header *eth_header = (struct ether_header *)packet;
    uint16_t eth_type = ntohs(eth_header->ether_type);

    // Variables to store packet information
    std::string src_ip, dst_ip, src_port = "0", dst_port = "0", proto_str;
    uint16_t prot_num = 0;                  // L3 Packet Protocol field in IPv4, Next Header field in IPv6
    uint16_t total_len = 0;                 // L3 Packet Total length field in IPv4, Payload length field in IPv6
                                            // These fields don't account for length of the Ethernet header and trailer
    const u_char *transport_header = nullptr;

    if (eth_type == ETHERTYPE_IP) {
        // IPv4 Packet
        const struct ip *ip_header = (struct ip *)(packet + sizeof(struct ether_header));
        parse_L3_ipv4(ip_header, src_ip, dst_ip, prot_num, total_len);                                            

        // Transport layer (L4) header
        transport_header = packet + sizeof(struct ether_header) + ip_header->ip_hl * 4;
    } else if (eth_type == ETHERTYPE_IPV6) {
        // IPv6 Packet
        const struct ip6_hdr *ip6_header = (struct ip6_hdr *)(packet + sizeof(struct ether_header));
        parse_L3_ipv6(ip6_header, src_ip, dst_ip, prot_num, total_len);

        // Transport layer (L4) header
        transport_header = packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr);
    } else {
        return;
    }

    parse_L4(prot_num, transport_header, proto_str, src_port, dst_port);

    if (proto_str.empty()) return; // Unsupported protocol

    // Create potential flow IDs
    FlowID tx_key = {src_ip, dst_ip, src_port, dst_port, proto_str};
    FlowID rx_key = {dst_ip, src_ip, dst_port, src_port, proto_str};

    update_flow_statistics(tx_key, rx_key, total_len);
}

/**
 * @brief Function for parsing IPv4 packet fields.
 * @param ip_header Pointer to the IPv4 header.
 * @param src_ip Source IP address as a string.
 * @param dst_ip Destination IP address as a string.
 * @param prot_num Protocol number.
 * @param total_len Total length of the packet.
 */
void parse_L3_ipv4(const struct ip *ip_header, std::string &src_ip, std::string &dst_ip, uint16_t &prot_num, uint16_t &total_len) {
    src_ip = inet_ntoa(ip_header->ip_src);       // Get source IP as a string
    dst_ip = inet_ntoa(ip_header->ip_dst);       // Get destination IP as a string
    prot_num = ip_header->ip_p;
    total_len = ntohs(ip_header->ip_len);
}

/**
 * @brief Function for parsing IPv6 packet fields.
 * @param ip6_header Pointer to the IPv6 header.
 * @param src_ip Source IP address as a string.
 * @param dst_ip Destination IP address as a string.
 * @param prot_num Protocol number.
 * @param total_len Total length of the packet.
 */
void parse_L3_ipv6(const struct ip6_hdr *ip6_header, std::string &src_ip, std::string &dst_ip, uint16_t &prot_num, uint16_t &total_len) {
    char src_ip_str[INET6_ADDRSTRLEN];
    char dst_ip_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(ip6_header->ip6_src), src_ip_str, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &(ip6_header->ip6_dst), dst_ip_str, INET6_ADDRSTRLEN);
    src_ip = src_ip_str;
    dst_ip = dst_ip_str;
    prot_num = ip6_header->ip6_nxt;
    total_len = ntohs(ip6_header->ip6_plen) + sizeof(struct ip6_hdr);
}

/**
 * @brief Function for parsing L4 protocol fields.
 * @param prot_num Protocol number.
 * @param transport_header Transport layer header.
 * @param proto_str Protocol name.
 * @param src_port Source port.
 * @param dst_port Destination port.
 */
void parse_L4(uint16_t prot_num, const u_char *transport_header, std::string &proto_str, std::string &src_port, std::string &dst_port) {
    if (prot_num == IPPROTO_TCP) {
        proto_str = "tcp";
        const struct tcphdr *tcp_header = (struct tcphdr *)transport_header;
        src_port = std::to_string(ntohs(tcp_header->th_sport));
        dst_port = std::to_string(ntohs(tcp_header->th_dport));
    } else if (prot_num == IPPROTO_UDP) {
        proto_str = "udp";
        const struct udphdr *udp_header = (struct udphdr *)transport_header;
        src_port = std::to_string(ntohs(udp_header->uh_sport));
        dst_port = std::to_string(ntohs(udp_header->uh_dport));
    } else if (prot_num == IPPROTO_ICMP) {
        proto_str = "icmp";
    } else if (prot_num == IPPROTO_ICMPV6) {
        proto_str = "icmp6";
    } else {
        proto_str = "";
    }
}
