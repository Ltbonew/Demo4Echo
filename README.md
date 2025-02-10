  <h2 align="center">AI语音助手DEMO</h2>



### :ledger: Overview

这个AI语音助手的项目可以完美部署到轻量级的Linux开发板上~本项目的AI语音助手的大致结构框图如下：

<p align="center">
	<img border="1px" width="50%" src="./res/AI_Audio_diagram.png">
</p>

[FSMN-VAD](https://www.modelscope.cn/models/iic/speech_fsmn_vad_zh-cn-16k-common-pytorch/summary): 阿里达摩院语音端点检测模型，用于检测语音活跃端点

<p align="center">
	<img border="1px" width="35%" src="./res/fsmn-diagram.jpg">
</p>

[SenceVoice](https://github.com/FunAudioLLM/SenseVoice): 阿里的音频处理模型，用于语音转文字(ASR)，情感识别(SER)等

<p align="center">
	<img border="1px" width="50%" src="./res/SenceVoice-diagram.jpg">
</p>

[CosyVoice](https://github.com/FunAudioLLM/CosyVoice):  语音生成大模型，用于文字转语音生成(TTS)

<p align="center">
	<img border="1px" width="50%" src="./res/CosyVoice-diagram.jpg">
</p>

[FastText](https://fasttext.cc/): Facebook于2016年开源的一个词向量计算和文本分类工具，这里用于获取说话人的指令进行一个多分类任务

### :file_folder:运行流程：

本项目的服务器Sever端，可以在没有GPU的笔记本电脑运行，除了通义千问和CosyVoice这两个生成式模型是调用阿里的API，其他的都是运行在本地的，当然如果有较强算力的同学，可以直接将这些模型都部署到本地服务器~

**注意：**如果需要自定义指令识别，可以重新改一下fastText的数据集，然后再训练得到自己的模型即可拿去用了，详见Sever端的文件夹中的内容。

AI语音助手的运行流程大致如下图所示：

<p align="center">
	<img border="1px" width="25%" src="./res/flow-chart.png">
</p>

### :bookmark_tabs:Websockets协议定义：

Client端和Server端通过Webscokets进行通信，通过JSON进行交互，语音传输格式为opus. Client端收到的消息，即Server端发送的消息，详见Server的README.

以下是Client端会向Server端发送的信息:

1. 鉴权信息：

   ```json
   Authorization: "Bearer " + access_token
   Device-Id: MAC address
   Protocol-Version: 定义的协议版本
   ```

2. 发送参数

   ```json
   {
       "type": "hello",
       "audio_params": {
           "format": "opus",
           "sample_rate": "16000",
           "channels": "1",
           "frame_duration": "40" // ms
       }
   }
   ```

3. 发送状态改变

   ```json
   {
       "type": "state", 
       "state": "idle" // 包括listening等，详见代码
   }
   ```

4. 打包发送的音频数据

   ```cpp
   struct BinProtocol {
       uint16_t version;       //协议版本
       uint16_t type;          //0为音频数据
       uint32_t payload_size;  //音频数据长度
       uint8_t payload[];      //opus音频数据
   } __attribute__((packed));
   ```

