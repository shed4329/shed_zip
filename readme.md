# shed_zip
## 关于这个项目
这是shed4329试图基于shed_std建立的一个压缩库，初步支持了解压和压缩，并且提供了一个简陋的cli程序以供交互
## 支持的东西
deflate算法，支持store，fixed huffman 还有 dynamic huffman等算法，在解压的时候
zip，gzip，raw deflate 都会在解压的时候自动尝试
## 问题
- 不支持加密
- 不支持分卷
- cli不支持大于64kb的文件
- 压缩的时候强制使用dynamic huffman
- 压缩文件解压只会考虑第一个，压缩也只能压缩一个文件
## 版本
shed_zip v0.02
基于 shed_std v0.16 beta 5
