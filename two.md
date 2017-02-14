###更改ui下的时区
   /home/wangyao/Code/Z6U110H/frameworks/base/packages/SystemUI/AndroidManifest.xml 中添加权限
         <uses-permission android:name="android.permission.SET_TIME_ZONE" />
   /home/wangyao/Code/Z6U110H/frameworks/base/packages/SystemUI/src/com/android/systemui/statusbar/phone/PhoneStatusBar.java中
   resetTimeZone(Context context)方法，添加对应ui的时区设置：
```
    if (ProjectManager.isUi18()) {
                timeZone = "Asia/Hovd";
            }
```
```
   Settings.System.putInt(context.getContentResolver(),"reset_time_zone",1); //设置系统属性，1重置，0否
   c中property_get("persist.sys.language", propLang, "");   //获取系统属性。
   char *strcpy(char* dest, const char *src);              //把从src地址开始且含有NULL结束符的字符串复制到以dest开始的地址空间
   char * strncat(char *dest, const char *src, size_t n); //把src所指字符串的前n个字符添加到dest所指字符串的结尾处，并覆盖dest所指字符串结尾的'\0'，从而实现字符串的连接。
```
