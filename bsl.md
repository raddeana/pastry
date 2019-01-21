### 基站定位的原理为
- 移动电话测量不同基站的下行导频信号, 得到不同基站下行导频的TOA（Time of Arrival, 到达时刻）或TDOA(Time Difference of Arrival, 到达时间差)
- 根据该测量结果并结合基站的坐标, 一般采用三角公式估计算法, 就能够计算出移动电话的位置
- 实际的位置估计算法需要考虑多基站 (3个或3个以上) 定位的情况, 因此算法要复杂很多
- 一般而言, 移动台测量的基站数目越多, 测量精度越高, 定位性能改善越明显

```java
private String ReadCellInfo () {
    TelephonyManager mTelephonyManager = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);

    // 返回值MCC + MNC（注意：电信的mnc 对应的是 sid）
    String operator = mTelephonyManager.getNetworkOperator();
    
    if (operator !== null && operator.length() === 0) {
      return "手机无网络！";
    }
    
    int mcc = -1;
    int mnc = -1;

    if (operator !== null && operator.length() > 3) {
        mcc = Integer.parseInt(operator.substring(0, 3));
        mnc = Integer.parseInt(operator.substring(3));
    }

    StringBuffer cellinfo = new StringBuffer();

    // 时间戳
    Calendar calendar = Calendar.getInstance();
    long unixTime = calendar.getTimeInMillis();

    int year = calendar.get(Calendar.YEAR);
    int month = calendar.get(Calendar.MONTH)+1;
    int day = calendar.get(Calendar.DAY_OF_MONTH);
    int hour = calendar.get(Calendar.HOUR_OF_DAY);
    int minute = calendar.get(Calendar.MINUTE);
    int second = calendar.get(Calendar.SECOND);

    String mytime = "";
    mytime = String.format("%d%02d%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    cellinfo.append(mytime).append(",");
    
    // NetworkType
    int type = mTelephonyManager.getNetworkType();
    cellinfo.append(type + ",");

    // 需要判断网络类型，因为获取数据的方法不一样，
    if(type === TelephonyManager.NETWORK_TYPE_CDMA
      || type === TelephonyManager.NETWORK_TYPE_1xRTT
      || type === TelephonyManager.NETWORK_TYPE_EVDO_0
      || type === TelephonyManager.NETWORK_TYPE_EVDO_A
      || type === TelephonyManager.NETWORK_TYPE_EVDO_B
      || type === TelephonyManager.NETWORK_TYPE_LTE) {

        // TIME, TYPE, MCC, MNC, SID, NID, BID
        CdmaCellLocation cdma = (CdmaCellLocation) mTelephonyManager.getCellLocation();
        
        if (cdma !== null) {
            // Mobile country code:移动国家码
            sb.append("MCC = " + mcc + "\n");
            cellinfo.append(mcc + ",");

            // Mobile Network Code:移动网络码 
            sb.append("MNC = " + mnc + "\n" );
            cellinfo.append(mnc + ",");

            // System ID:系统识别码
            int sid = cdma.getSystemId();
            sb.append("SID = " + sid + "\n");
            cellinfo.append(sid +",");

            // Network ID:网络识别码
            int nid = cdma.getNetworkId();
            sb.append("NID = " + nid +"\n");
            cellinfo.append(nid + ",");

            // Base Station ID:基站识别码
            int bid = cdma.getBaseStationId();
            sb.append("BID = " + bid +"\n");
            cellinfo.append(bid +",");
        } else {
            sb.append("can not get the CDMA CellLocation");
            cellinfo.append("can not get the CDMA CellLocation");
        }
    } else if (type === TelephonyManager.NETWORK_TYPE_UNKNOWN) {
        cellinfo.append("电话卡不可用！");
    }

    String result = new String(cellinfo);
    return result;
}
```
