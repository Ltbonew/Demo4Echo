import asyncio
import websockets
import json
from MessageHandler import handle_message

# 预定义的鉴权信息
ACCESS_TOKEN = "123456"
DEVICE_ID = "00:11:22:33:44:55"
PROTOCOL_VERSION = "1"

# 用于存储已验证的客户端连接
verified_clients = set()

# 配置日志
import logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# 客户端鉴权函数
async def authenticate(headers):
    # 验证 token, device_id, protocol_version 是否正确
    if (headers.get("Authorization") == "Bearer " + ACCESS_TOKEN and
        headers.get("Device-Id") == DEVICE_ID and
        headers.get("Protocol-Version") == PROTOCOL_VERSION):
        return True
    else:
        return False

# 处理客户端连接
async def handle_client(websocket, path):
    # 获取连接时的请求头
    headers = websocket.request_headers

    # 执行鉴权，若不通过则关闭连接
    authenticated = await authenticate(headers)
    if not authenticated:
        await websocket.send(json.dumps({"type": "error", "message": "Authentication failed"}))
        await websocket.close(reason="Authentication failed")
        logger.error("Authentication failed for client")
        return

    # 鉴权通过后，将连接加入已验证集合
    verified_clients.add(websocket)
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
            if websocket not in verified_clients:
                # 若连接未验证，则关闭连接
                await websocket.close(reason="Unverified client")
                logger.warning("Closing connection for unverified client")
                return
            else:
                # 处理客户端消息
                response = await handle_message(message)
                if response is not None:
                    if isinstance(response, bytes):  # 如果响应是二进制数据
                        await websocket.send(response)
                    else:  # 如果响应是文本数据
                        await websocket.send(json.dumps(response))

    except websockets.exceptions.ConnectionClosed as e:
        logger.warning(f"Connection closed unexpectedly: {e}")
    finally:
        # 连接关闭后从已验证集合中移除
        verified_clients.discard(websocket)
        logger.info("Client disconnected")

# 启动 WebSocket 服务器
async def start_server():
    async with websockets.serve(handle_client, "0.0.0.0", 8765):
        logger.info("Server started on port 8765")
        await asyncio.Future()  # 保持服务器运行

# 运行服务器
if __name__ == "__main__":
    asyncio.run(start_server())
