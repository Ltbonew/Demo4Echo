import asyncio
import logging
import websockets
import json
from MessageHandler import MessageHandler

# 预定义的鉴权信息
ACCESS_TOKEN = "123456"
DEVICE_ID = "00:11:22:33:44:55"
PROTOCOL_VERSION = "1"
HOST = "0.0.0.0"
PORT = 8765

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class WebSocketServer:
    def __init__(self, host="0.0.0.0", port=8765, access_token="123456", device_id="00:11:22:33:44:55", protocol_version="1"):
        self.host = host
        self.port = port
        self.access_token = access_token
        self.device_id = device_id
        self.protocol_version = protocol_version
        self.verified_clients = set()
        self.message_handler = MessageHandler()

    # 客户端鉴权函数
    async def authenticate(self, headers):
        # 验证 token, device_id, protocol_version 是否正确
        if (headers.get("Authorization") == "Bearer " + self.access_token and
            headers.get("Device-Id") == self.device_id and
            headers.get("Protocol-Version") == self.protocol_version):
            return True
        else:
            return False

    # 处理客户端连接
    async def handle_client(self, websocket, path):
        # 获取连接时的请求头
        headers = websocket.request_headers

        # 执行鉴权，若不通过则关闭连接
        authenticated = await self.authenticate(headers)
        if not authenticated:
            await websocket.send(json.dumps({"type": "error", "message": "Authentication failed"}))
            await websocket.close(reason="Authentication failed")
            logger.error("Authentication failed for client")
            return

        # 鉴权通过后，将连接加入已验证集合
        self.verified_clients.add(websocket)
        logger.info("Client authenticated")

        # 向客户端发送成功响应
        response = {
            "type": "hello",
            "message": "Client authenticated",
        }
        await websocket.send(json.dumps(response))

        try:
            # 开始接收和处理客户端消息
            async for message in websocket:
                if websocket not in self.verified_clients:
                    # 若连接未验证，则关闭连接
                    await websocket.close(reason="Unverified client")
                    logger.warning("Closing connection for unverified client")
                    return
                else:
                    # 处理客户端消息
                    response = await self.message_handler.handle_message(message)
                    if response is not None:
                        if isinstance(response, bytes):  # 如果响应是二进制数据
                            await websocket.send(response)
                        else:  # 如果响应是文本数据
                            await websocket.send(json.dumps(response))

        except websockets.exceptions.ConnectionClosed as e:
            logger.warning(f"Connection closed unexpectedly: {e}")
        finally:
            # 连接关闭后从已验证集合中移除
            self.verified_clients.discard(websocket)
            logger.info("Client disconnected")

    # 启动 WebSocket 服务器
    async def start_server(self):
        async with websockets.serve(self.handle_client, self.host, self.port):
            logger.info(f"Server started on {self.host}:{self.port}")
            await asyncio.Future()  # 保持服务器运行

# 运行服务器
if __name__ == "__main__":
    server = WebSocketServer(host="0.0.0.0", port=8765)
    asyncio.run(server.start_server())
