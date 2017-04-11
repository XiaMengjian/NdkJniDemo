# NdkJniDemo
so 库进行签名认证 私钥管理

获取签名的方法：

```
    public static String getSignature(Context context) {
        try {
            /** 通过包管理器获得指定包名包含签名的包信息 **/
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context
                    .getPackageName(), PackageManager.GET_SIGNATURES);
            /******* 通过返回的包信息获得签名数组 *******/
            Signature[] signatures = packageInfo.signatures;
            /******* 循环遍历签名数组拼接应用签名 *******/
            return signatures[0].toCharsString();
            /************** 得到应用签名 **************/
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }
```

### 注意： 请修改cpp 文件中类名 为你工程目录 path 名称

具体内容 请查阅[Mohon](http://mohon.win/2017/04/11/Android-%E4%BD%BF%E7%94%A8-so-%E8%BF%9B%E8%A1%8C%E7%AD%BE%E5%90%8D%E9%AA%8C%E8%AF%81%E4%BB%A5%E5%8F%8A%E5%AD%98%E6%94%BE%E7%A7%81%E9%92%A5/)
