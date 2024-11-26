import asyncio
from WebsocketServer import WebSocketServer

# 运行服务器
if __name__ == "__main__":
    server = WebSocketServer()
    asyncio.run(server.start_server())
