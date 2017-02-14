 /Code 是工作目录
 /Z6U110H是本次项目
### 添加开机第一帧图片（第二帧同第一帧一样）
     添加图片 hd720_kernel_18.bmp ，18 是logo_id
     在 /Code/Z6U110H/device/gxq/gxq6580_weg_l/ProjectConfig.mk中找到 BOOT_LOGO = hd720,
    hd720是/Code/Z6U110H/bootable/bootloader/lk/dev/logo/中的一个文件夹，存放一些图片包括开机第一帧图片。（hd720为屏幕的大小，720*1280）
    将图片放入hd720目录中，回到logo文件夹中 在rules.mk 中添加$(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_kernel_18.raw \ （logo_id要对应），在update文件中添加
    ./tool/bmp_to_raw ./boot_logo ./$p/"${p}_kernel_18".bmp 
     
    编译lk模块，
    make clean-lk(清除之前编译的)
    make lk （编译）
    使用flash tools刷入手机。
***
### 添加开机动画
####制作开机动画包：
    将hd720_kernel_18.bmp图片作为开机动画，创建part0文件夹，并将图片放入，创建part1文件夹，并将图片放入，
    编辑desc.txt 
    720 1280 15  720是宽，1280是高，不解释。意思就是说把定义在下面p部分的图片拉伸或缩放到这个比例，你做的时候最好把你的图片和你手机分辨率等比例转换，，，15表示每秒放15桢，即15张图
    p 1 0 part0  第一位“p” 是指定义一个部分，这里定义的是part0，第二位“1” 表示动画重复次数，1表示播放一次，也可以为0，表示无限循环；第三位“0”表示本部分播放完后接下部分中间延迟间隔，一般为0
    p 0 0 part1  同上。这里第二位就用了数字0，表示part1下面的图片是按照无限循环来播放的

    在windows中将part0，part1，desc.txt 选中通过winRaR压缩，选中不压缩，格式为zip，命名为bootanimation18.zip即可.(关机动画同理)
***
### 开机声音，同开机动画
***
####添加开机动画
    将动画包放入/Code/Z6U110H/packages/apps/BootOrShutAnim 文件夹中，并在BootOrShutAnim.mk中添加 $(LOCAL_PATH)/shutanimation18.zip:system/media/shutanimation18.zip \
    编译：
    make clean-systemimage
    make systemimage
    刷机重启。
     (可通过adb push 直接将动画包 push 进/system/media/ 中)
***
###添加指令
   找到/Code/Z6U110H/packages/apps/Dialer/src/com/android/dialer/SpecialCharSequenceMgr.java ,
   在 handleUserIdLogoSwitch(Context context, String input) 方法中添加对应的logo_id和对应的切换指令。
   默认的ui，logo_id 可以在 /Code/Z6U110H/device/gxq/gxq6580_weg_l/ProjectConfig.mk中找到 CUSTOM_DEFAULT_UI=18 更改
***
###更改机型蓝牙名称、wifi名称、pc端名称、照片型号同步修改
   /Code/Z6U110H/frameworks/base/core/java/android/os/ProjectManager.java 中找到
   getBrand()方法，添加对应logo_id 的品牌。
   getProductName()，产品名。
   getDeviceMode() ，手机型号。
   getWifiName() ，wifi名。
   getBlueToothName()，和isDefaultBluetoothName(String name)，蓝牙名。

### 有关/home/wangyao/Code/Z6U110H/n_Z6U110HC_En.sh编译脚步
```
   #update                                               //同步
   #svn cleanup
   #svn up

    chmod 777 -R ./

   #copy files                                          //  将/home/wangyao/Code/Z6U110H/Z6U110HC中文件替换到原工程目录中
    cp -rf ./Z6U110HC tmp
    find ./tmp -type d -name ".svn"|xargs rm -rf
    cp -rfv ./tmp/* ./
    rm -rf tmp

    source build/envsetup.sh                             //编译环境
    lunch full_gxq6580_weg_l-eng
    #lunch full_gxq6580_weg_l-user

    #make clean -j8 2>&1 | tee build.log
     make -j8 2>&1 | tee build.log


 
    
    

    

