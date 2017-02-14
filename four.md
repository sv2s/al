##Android Widget（简单的日历小组件CalendarWidget）
1. 创建AppWidgetProvider的实现类CalendarWidgetProvider.java;

 在AndroidManifest.xml中添加recevier节点
```
<receiver android:name=".CalendarWidgetProvoder">
            <intent-filter>
                 //必须的
                <action android:name="android.appwidget.action.APPWIDGET_UPDATE" />

                 //自定义的，用来接受按钮的点击
                 <action android:name="com.example.wangyao.REFRESH_BUTTON"/>
                 //接受gridview item的点击
                 <action android:name="com.example.wangyao.GRIDVIEW_CLICK"/>
            </intent-filter>
            <meta-data
                android:name="android.appwidget.provider"

                //一个xml文件，一些配置
                android:resource="@xml/calendar_widget_info" />
</receiver>
```
2. 创建xml文件calendar_widget_info.xml
```
<?xml version="1.0" encoding="utf-8"?>
<appwidget-provider xmlns:android="http://schemas.android.com/apk/res/android"
    android:initialKeyguardLayout="@layout/calendar_widget"                              //布局文件，支持的layout，view较少
    android:initialLayout="@layout/calendar_widget"
    android:minHeight="250dp"                                                            // 70*n-30(n=4)
    android:minWidth="250dp"                                                             // 70*n-30(n=4)
    android:previewImage="@drawable/calendar_widget_preview"                             //预览图片
    android:resizeMode="horizontal|vertical" 
    android:updatePeriodMillis="100"
    android:widgetCategory="home_screen">
</appwidget-provider>
```

3. 编辑布局
####   RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.calendar_widget);
4. 普通button添加点击：
```
        Intent intent = new Intent();
        intent.setAction(REFRESH_BUTTON);                     // REFRESH_BUTTON = com.example.wangyao.REFRESH_BUTTON
        PendingIntent btp = PendingIntent.getBroadcast(context, 1,intent,PendingIntent.FLAG_UPDATE_CURRENT);
        views.setOnClickPendingIntent(R.id.refresh_btn,btp);

```

在provider的onreciver处理；

### gridview的数据和点击。

1. 创建RemoteViewsService的实现类 MyCalendarService
2. 创建RemoteViewsService.RemoteViewsFactory实现类MyFactory（类似adapter）
3. 在AndroidManifest.xml中添加service节点
```
 <service android:name=".MyCalendarService"
            android:permission="android.permission.BIND_REMOTEVIEWS"/>
```

4. 在provider中：
```
        Intent intentService = new Intent(context,MyCalendarService.class);
        intentService.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID,appWidgetId);
        remoteViews.setRemoteAdapter(R.id.grid,intentService);
```
  点击事件还需加入：
```
remoteViews.setPendingIntentTemplate(R.id.grid,PendingIntent.getBroadcast(context,0,
                   new Intent()
                        .setAction(COLLECTION_VIEW_ACTION)
                        .setClass(context,this.getClass())
                        .putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID,appWidgetId)
                        ,PendingIntent.FLAG_UPDATE_CURRENT));
```
   
更新gridview中数据通过appWidgetManager.notifyAppWidgetViewDataChanged(appWidgetId, R.id.grid);


###日历小组件：
1获取小组件的最小宽高：
```
 	    int minWidthDp = widgetOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MIN_WIDTH);
            int minHeightDp = widgetOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MIN_HEIGHT);
            //显示样式
            // 小于 240(4格)，年月显示变短
            shortMonthName = minWidthDp <= 240 ;
            //小于180（3格），不显示年月
            mini = minHeightDp <= 180 ;
            if(mini){
                numweeks = minHeightDp <= 120 ? 1 : 2;
            }
```

创建日历类实例Calendar cal = Calendar.getInstance();

分别获取不同模式下的月份。并判断SharedPreference中有无保存的值，设置相应的日历；

点击下个月按钮，发送广播，月份+1，存入SharedPreference，

在gridview更新数据时，读出模式。

github:https://github.com/sv2sv/MyCalendarWidget.git







