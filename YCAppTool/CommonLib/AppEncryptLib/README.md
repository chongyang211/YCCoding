# 加密和解密


### 03.Api调用说明
- 关于MD5加密Api如下所示
    ``` java
    //对字符串md5加密
    String md2 = Md5EncryptUtils.encryptMD5ToString(string);
    //对字符串md5加密，加盐处理
    String md3 = Md5EncryptUtils.encryptMD5ToString(string,"doubi");
    //对字节数据md5加密
    String md4 = Md5EncryptUtils.encryptMD5ToString(bytes);
    //对字节数据md5加密，加盐处理
    String md5 = Md5EncryptUtils.encryptMD5ToString(bytes,"doubi".getBytes());
    //对文件进行md5加密
    String md6 = Md5EncryptUtils.encryptMD5File1(txt);
    //对文件进行md5加密
    String md7 = Md5EncryptUtils.encryptMD5File2(new File(txt));
    ```
- 关于base64加密和解密的Api如下所示
    ``` java
    //字符Base64加密
    String strBase64_2 = Base64Utils.encodeToString(str);
    //字符Base64解密
    String strBase64_3 = Base64Utils.decodeToString(strBase64_2);
    ```
- 关于DES加密和解密的Api如下所示
    ``` java
    //DES加密字符串
    String encrypt1 = DesEncryptUtils.encrypt(string,password);
    //DES解密字符串
    String decrypt1 = DesEncryptUtils.decrypt(encrypt1 , password);
    //DES加密文件
    String encryptFile1 = DesEncryptUtils.encryptFile(password, fileName, destFile);
    //DES解密文件
    String decryptFile1 = DesEncryptUtils.decryptFile(password, destFile, destFile3);
    //DES 加密后转为 Base64 编码
    String encrypt2 = DesEncryptUtils.encrypt(string.getBytes(), password.getBytes());
    //DES解密字符串 Base64 解码
    String decrypt2 = DesEncryptUtils.decrypt(encrypt2.getBytes(), password.getBytes());
    ```
- 关于AES加密和解密的Api如下所示
    ``` java
    //AES加密字符串
    String encrypt1 = AesEncryptUtils.encrypt(string,password);
    //AES解密字符串
    String decrypt1 = AesEncryptUtils.decrypt(encrypt1 , password);
    ```
- 关于RC4加密和解密的Api如下所示
    ``` java
    //RC4加密
    String encrypt1 = Rc4EncryptUtils.encryptString(string, secretKey);
    //RC4解密
    String decrypt1 = Rc4EncryptUtils.decryptString(encrypt1, secretKey);
    //RC4加密base64编码数据
    String encrypt2 = Rc4EncryptUtils.encryptToBase64(bytes1, secretKey);
    //RC4解密base64解码数据
    byte[] bytes = Rc4EncryptUtils.decryptFromBase64(encrypt2, secretKey);
    ```








