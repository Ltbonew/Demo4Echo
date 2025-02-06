import asyncio
import queue
import threading
import time
import logging
from WebsocketServer import WebSocketServer
from MessageHandler import MessageHandler

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# 运行服务器
if __name__ == "__main__":
    ws_rec_queue = queue.Queue()
    ws_send_queue = queue.Queue()
    stop_queue = queue.Queue()
    websocket_server = WebSocketServer(ws_rec_msg=ws_rec_queue, ws_send_msg=ws_send_queue)
    msg_handler = MessageHandler(ws_rec_msg=ws_rec_queue, ws_send_msg=ws_send_queue)

    async def ws_tasks_create():
        websocket_task = asyncio.create_task(websocket_server.ws_server_task_run())
        ws_msg_send_task = asyncio.create_task(websocket_server.ws_msg_send_task_run())
        while True:
            if not stop_queue.empty():
                logger.info("Stopping WebSocket tasks...")
                websocket_task.cancel()
                ws_msg_send_task.cancel()
                break
            await asyncio.sleep(0.1)  # 定期检查停止队列
        # 等待任务取消完成
        try:
            await asyncio.gather(websocket_task, ws_msg_send_task, return_exceptions=True)
        except asyncio.CancelledError:
            logger.info("WebSocket tasks have been cancelled.")

    def websocket_thread_run():
        asyncio.run(ws_tasks_create())

    def msg_handler_thread_run():
        while True:
            # 如果收到停止信号则退出
            if not stop_queue.empty():
                logger.info("Stopping message handler thread")
                break
            msg_handler.handle_message_task_run()
            time.sleep(0.1)
    # 创建线程
    ws_thread = threading.Thread(target=websocket_thread_run)
    msg_handler_thread = threading.Thread(target=msg_handler_thread_run)
    # 启动线程
    ws_thread.start()
    msg_handler_thread.start()
    try:
        while True:
            # do nothing
            time.sleep(1)
    # 捕获键盘 ctrl+c 中断信号
    except KeyboardInterrupt:
        stop_queue.put(True)
        logger.info("Shutting down...")
    # 等待线程结束
    ws_thread.join()
    msg_handler_thread.join()
    logger.info("Done")
