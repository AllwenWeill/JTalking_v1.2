# JTalking_v1.2
JTalking（Just Talking）是基于QT开发的一款即时通讯软件，具有登录、注册、聊天、传输文件功能，在登录部分采用CS模型，需将用户注册账号信息打包发送给服务器端，服务器拆包识别各字段含义，将客户端的账号密码端口号等信息添加在服务器数据库中，客户端可通过刷新按钮可查询服务器端数据库的好友信息，然后与指定好友进行P2P连接，不再让服务器端当成中转站，服务器端并没有采用多线程处理多个客户端，而是采用分时方案减轻服务器端的压力。
---

**登录页面**

![image](https://github.com/AllwenWeill/IMG/blob/main/%E6%88%AA%E5%9B%BE11.png)

**注册页面**

![image](https://github.com/AllwenWeill/IMG/blob/main/%E6%88%AA%E5%9B%BE12.png)

**聊天页面**

![image](https://github.com/AllwenWeill/IMG/blob/main/JTalking-%E8%81%8A%E5%A4%A9%E9%A1%B5%E9%9D%A2.png)

**文件传输**

![image](https://github.com/AllwenWeill/IMG/blob/main/JTalking-%E6%96%87%E4%BB%B6%E4%BC%A0%E8%BE%93.png)

**服务器端页面**

![image](https://github.com/AllwenWeill/IMG/blob/main/JTalking-%E6%9C%8D%E5%8A%A1%E5%99%A8%E7%AB%AF%E9%A1%B5%E9%9D%A2.png)

---
**更新**
2022.5.01————更新服务器端页面，增加数据库显示结果和删除效果，增加日志记录；

2022.4.29————修复客户端弹出多次错误弹窗Bug；

2022.4.28————增加文件选择、读取传输功能；

