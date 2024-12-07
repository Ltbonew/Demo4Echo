#include "../inc/WebsocketClient.h"
#include <websocketpp/common/asio.hpp>

// WebSocketClient 构造函数
WebSocketClient::WebSocketClient(const std::string& address, int port, const std::string& token, const std::string& deviceId, const std::string& protocolVersion) {
    ws_client_.init_asio();
    ws_client_.set_open_handler(bind(&WebSocketClient::on_open, this, std::placeholders::_1));
    ws_client_.set_message_handler(bind(&WebSocketClient::on_message, this, std::placeholders::_1, std::placeholders::_2));
    ws_client_.set_close_handler(bind(&WebSocketClient::on_close, this, std::placeholders::_1)); // 设置关闭处理器

    uri_ = "ws://" + address + ":" + std::to_string(port);

    headers_["Authorization"] = "Bearer " + token;
    headers_["Device-Id"] = deviceId;
    headers_["Protocol-Version"] = protocolVersion;

}

WebSocketClient::~WebSocketClient() {
    Close();
    Terminate();
}

// 日志记录方法
void WebSocketClient::Log(const std::string& message, LogLevel level) {
    // 根据日志级别选择前缀
    std::string prefix;
    switch (level) {
        case LogLevel::INFO:
            prefix = "[INFO] ";
            break;
        case LogLevel::WARNING:
            prefix = "[WARNING] ";
            break;
        case LogLevel::ERROR:
            prefix = "[ERROR] ";
            break;
        default:
            prefix = "[UNKNOWN] ";
            break;
    }

    // 输出日志信息
    std::cout << "[WebSocketClient] " << prefix << message << std::endl;
}

// 会独立运行run()的，主要是避免阻塞
void WebSocketClient::Run() {
    ws_client_.start_perpetual();
    thread_ = std::make_shared<std::thread>([this]() {
        ws_client_.run();
        Log("WebSocket client thread ended.", LogLevel::INFO);
    });
}

// 连接函数
void WebSocketClient::Connect() {
    if (is_connected_) {
        Log("Already connected.", LogLevel::INFO);
        return;
    }
    websocketpp::lib::error_code ec;
    client_t::connection_ptr con = ws_client_.get_connection(uri_, ec);
    if (ec) {
        Log("Could not create connection: " + ec.message(), LogLevel::ERROR);
        return;
    }

    for (const auto& header : headers_) {
        con->append_header(header.first, header.second);
    }
    connection_hdl_ = con->get_handle();
    ws_client_.connect(con);
}

void WebSocketClient::Terminate() {
    try {
        ws_client_.stop_perpetual();
        thread_->join();
    }
    catch (const websocketpp::exception& e) {
        // 捕获并处理异常
        std::cerr << "WebSocket Exception: " << e.what() << std::endl;
    }
}

void WebSocketClient::Close() {
    try {
        ws_client_.close(connection_hdl_, websocketpp::close::status::going_away, "Client is being destroyed");
    } catch (const std::exception& e) {
        Log("Error closing connection: " + std::string(e.what()), LogLevel::ERROR);
    }
    
}

// 发送文本消息
void WebSocketClient::SendText(const std::string& message) {
    ws_client_.send(connection_hdl_, message, websocketpp::frame::opcode::text);
}

// 发送二进制数据
void WebSocketClient::SendBinary(const uint8_t* data, size_t size) {
    ws_client_.send(connection_hdl_, data, size, websocketpp::frame::opcode::binary);
}

// 设置消息回调
void WebSocketClient::SetMessageCallback(message_callback_t callback) {
    on_message_ = callback;
}

// 设置关闭回调
void WebSocketClient::SetCloseCallback(close_callback_t callback) {
    on_close_ = callback;
}

// 连接打开的回调
void WebSocketClient::on_open(websocketpp::connection_hdl hdl) {
    connection_hdl_ = hdl;
    Log("Connection established.", LogLevel::INFO);
    is_connected_ = true;
}

// 消息接收的回调
void WebSocketClient::on_message(websocketpp::connection_hdl hdl, client_t::message_ptr msg) {
    if (on_message_) {
        on_message_(msg->get_payload());
    }
}

// 连接关闭的回调
void WebSocketClient::on_close(websocketpp::connection_hdl hdl) {
    Log("Connection closed.", LogLevel::INFO);
    if (on_close_) {  // 调用用户设置的关闭回调
        on_close_();
    }
    is_connected_ = false;
}
