# S-DES
重庆大学 信息安全导论 第一次作业
本项目为C++编写的控制台程序，为八位二进制数组及文字（按ASCII码转换）提供S-DES加解密服务

加解密依靠使用10位原密钥。加密时，用户可以输入自己已有的轮密钥，也可让程序自动生成；在用户遗忘原密钥时，程序提供破解服务：输入明文和密文，程序将破解所有可能的密钥

1.加解密：
===
测试1
![image](https://github.com/surftillend/S-DES/blob/main/readmePicture/selfCheck0.png)
测试2：
![image](https://github.com/surftillend/S-DES/blob/main/readmePicture/selfCheck1.png)

2.交叉检查
===
与测试结果1对照：
---
胡海波老师班 [唐欣然组](https://github.com/txr1002/encryption)
![image](https://github.com/surftillend/S-DES/blob/main/readmePicture/crossCheck0.jpg)
胡海波老师班 [陈伟哲组（原神启动组）](https://github.com/star-night-rain/simple-DES)
![image](https://github.com/surftillend/S-DES/blob/main/readmePicture/crossCheck1.jpg)

与测试结果2对照：
---
向宏老师班 [许诺组（人民当家做组）](https://github.com/221al-Qaeda/S-DES-encrypt-and-decrypt)
（测试密钥1010000010）
![image](https://github.com/surftillend/S-DES/blob/main/readmePicture/crossCheck2.jpg)

3.破解密钥与封闭测试:
===
![image](https://github.com/surftillend/S-DES/blob/main/readmePicture/cracking0.png)
