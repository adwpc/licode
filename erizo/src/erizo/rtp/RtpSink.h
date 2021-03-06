/*
 * RtpSink.h
 *
 *  Created on: Aug 2, 2012
 *      Author: pedro
 */
#ifndef ERIZO_SRC_ERIZO_RTP_RTPSINK_H_
#define ERIZO_SRC_ERIZO_RTP_RTPSINK_H_

#include <asio.hpp>
#include <mutex>
#include <thread>
#include <condition_variable>

#include <queue>
#include <string>

#include "./MediaDefinitions.h"
#include "./logger.h"

namespace erizo {

class RtpSink: public MediaSink, public FeedbackSource {
  DECLARE_LOGGER();

 public:
  RtpSink(const std::string& url, const std::string& port, int feedbackPort);
  virtual ~RtpSink();

 private:
  std::unique_ptr<asio::ip::udp::socket> socket_, fbSocket_;
  std::unique_ptr<asio::ip::udp::resolver> resolver_;

  std::unique_ptr<asio::ip::udp::resolver::query> query_;
  asio::ip::udp::resolver::iterator iterator_;
  asio::io_service io_service_;

  std::thread send_Thread_, receive_Thread_;
  std::condition_variable cond_;
  std::mutex queueMutex_;
  std::queue<DataPacket> sendQueue_;
  bool sending_;

  static const int LENGTH = 1500;
  char* buffer_[LENGTH];

  int deliverAudioData_(std::shared_ptr<DataPacket> audio_packet) override;
  int deliverVideoData_(std::shared_ptr<DataPacket> video_packet) override;
  int sendData(char* buffer, int len);
  void sendLoop();
  void serviceLoop();
  void handleReceive(const asio::error_code& error, size_t bytes_recvd);  // NOLINT
  void queueData(const char* buffer, int len, packetType type);
};
}  // namespace erizo
#endif  // ERIZO_SRC_ERIZO_RTP_RTPSINK_H_
