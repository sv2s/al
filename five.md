### 更改时区
切换ui时加入：
//重置时区标志
Settings.System.putInt(context.getContentResolver(),"reset_time_zone",1);
并在/frameworks/base/packages/SystemUI/src/com/android/systemui/statusbar/phone/PhoneStatusBar.java

resetTimeZone(Context context)方法中添加对应ui的时区

```
           if (ProjectManager.isUi23()){
                timeZone = "Asia/Bangkok";
            }
```

## svn

svn update filename 更新

svn ci 提交

svn st 比对

svn add 添加
***
svn diff    比较本地工作拷贝的修改

svn diff -r 3 filename 比较本地工作拷贝与版本库指定版本

svn diff -r 2:3 $URL比较版本库的两个版本

### 语言列表中添加语言
/home/wangyao/Code/Z6U231H/device/gxq/gxq6580_weg_l/ProjectConfig.mk 中

MTK_PRODUCT_LOCALES =th_TH en_US zh_CN zh_TW es_ES pt_BR ru_RU fr_FR de_DE tr_TR vi_VN ms_MY in_ID it_IT ar_EG hi_IN bn_IN ur_PK fa_IR pt_PT nl_NL el_GR hu_HU tl_PH ro_RO cs_CZ ko_KR km_KH iw_IL pl_PL es_US bg_BG hr_HR lv_LV lt_LT sk_SK uk_UA de_AT da_DK fi_FI nb_NO sv_SE en_GB hy_AM et_EE ja_JP kk_KZ sr_RS sl_SI ca_ES my_ZG lo_LA 列表

若想显示中文 还需frameworks/base/core/java/com/android/internal/app/LocalePicker.java中isLanguageCN方法添加

```
if(FeatureOption.CUSTOM_PROJECT_A||FeatureOption.CUSTOM_PROJECT_D){
                    return false;//默认为开启
                } else{
                    return true;
                }	
```



