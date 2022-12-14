/// Please see camera development documentation.

#include <CameraDefine.h>
#include <zconf.h>
#include "MVCamera.h"
#include <ros/ros.h>

#include <signal.h>
 
void MySigintHandler(int sig)
{
	//这里主要进行退出前的数据保存、内存清理、告知其他节点等工作
	ROS_INFO("shutting down!");
	ros::shutdown();
}


MVCamera::MVCamera()
{

    iCameraCounts = 4;
    iStatus = 0;
    hCamera = 0;
    channel = 3;

    //tSdkCameraDevInfo       MVCamera::tCameraEnumList[4];
    tCapability;      //设备描述信息
    sFrameInfo;
    g_pRgbBuffer[0] = NULL;
    g_pRgbBuffer[1] = NULL;
    ready_buffer_id = 0;
    stopped = false;
    updated = false;
}

int MVCamera::Init(int id)
{
    printf("CAMERA SDK INIT...\n");
    CameraSdkInit(1);
    printf("DONE\n");


    printf("ENUM CAMERA DEVICES...\n");
    //枚举设备，并建立设备列表
    tSdkCameraDevInfo tCameraEnumList[4];
    CameraEnumerateDevice(tCameraEnumList,&iCameraCounts);
    //没有连接设备
    if(iCameraCounts==0)
    {
        printf("ERROR: NO CAMERA CONNECTED.\n");
        return -1;
    } 
    // else if (iCameraCounts <= id) 
    // {
    //     printf("CONNECTED CAMERA NUMBERS: %d TOO SMALL, ID: %d.\n", iCameraCounts, id);
    //     return -1;
    // } 
    else
     {
        printf("CONNECTED CAMERA NUMBERS: %d.\n", iCameraCounts);
    }
    printf("DONE\n");

    if(iCameraCounts>1)
    {
        string ns = ros::this_node::getNamespace();
        string camName;
        for(int i=0; i < iCameraCounts; i++) {
            camName = string(tCameraEnumList[i].acFriendlyName);
            cout<<camName<<endl;
            if (camName[camName.size() - 1] == ns[ns.size() - 1]) {
            	id =i;
                break;
            }
        }
        /*for(int i=0;i<iCameraCounts;i++)
        {
            CameraInit(&(tCameraEnumList[0]),-1,-1,&hCamera);
            string ns = ros::this_node::getNamespace();
            BYTE name[1000]={0};
            CameraReadSN(hCamera,name,0);
            cout<<name[2]<<endl;
            if(ns =="up"&& name[2] ==9)
            {
                id = i;
                break;
            }
            if(ns =="down"&& name[2] ==0)
            {
                id = i;
                break;
            }
            CameraUnInit(hCamera);
        }*/
    }
    
    //相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
    iStatus = CameraInit(&(tCameraEnumList[id]),-1,-1,&hCamera);
    //初始化失败
    if (iStatus!=CAMERA_STATUS_SUCCESS) {
        printf("ERROR: CAMERA INIT FAILED.\n");
        return -1;
    } else {
        printf("CAMERA INIT SUCCESS.\n");
    }

    //设置色温模式
    //  iStatus = CameraSetPresetClrTemp(hCamera, 1);
    //	if (iStatus == CAMERA_STATUS_SUCCESS) {
    //    printf("CAMERA SETPRESETCLRTEMP SUCCESS!\n");
    //	} else {
    //    printf("CAMERA SETPRESETCLRTEMP FAILED! ERROR CODE: %d\n", iStatus);
    //	}

    //  iStatus = CameraSetClrTempMode(hCamera, 1);
    //	if (iStatus == CAMERA_STATUS_SUCCESS) {
    //    printf("CAMERA SETCLRTEMPMODE SUCCESS!\n");
    //	} else {
    //    printf("CAMERA SETCLRTEMPMODE FAILED! ERROR CODE: %d\n", iStatus);
    //	}

    //获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
//    CameraGetCapability(hCamera,&tCapability);
//    CameraPlay(hCamera);

    //设置输出为彩色
    channel = 3;
    CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_BGR8);


    started=1;
    return 0;
}

int MVCamera::Uninit()
{
    printf("Save Parameter...\n");
    CameraSaveParameter(hCamera, 0);

    printf("Uninit...\n");
    int status = CameraUnInit(hCamera);

    printf("status: %d\n", status);

    if (status == CAMERA_STATUS_SUCCESS) {
        printf("CAMERA UNINIT SUCCESS!\n");
    } else {
        printf("CAMERA UNINIT FAILED! ERROR CODE: %d\n", status);
    }

    if (g_pRgbBuffer[0] != NULL) {
        free(g_pRgbBuffer[0]);
        g_pRgbBuffer[0] = NULL;
    }

    if (g_pRgbBuffer[1] != NULL) {
        free(g_pRgbBuffer[1]);
        g_pRgbBuffer[1] = NULL;
    }
    started=0;
}

void MVCamera::CheckConnect()
{
    // if(!CameraConnectTest(hCamera))
    // {
    //     return;
    // }

    ROS_WARN("CAMERA RECONNECT!!!!");
        
}

int MVCamera::SetExposureTime(bool auto_exp, double exp_time)
{
    if (auto_exp) {
        CameraSdkStatus status = CameraSetAeState(hCamera, true);
        if (status == CAMERA_STATUS_SUCCESS) {
            printf("ENABLE AUTO EXP SUCCESS.\n");
        } else {
            printf("ENABLE AUTO EXP FAILED.\n");
            return status;
        }
    } else {
        CameraSdkStatus status = CameraSetAeState(hCamera, false);
        if (status == CAMERA_STATUS_SUCCESS) {
            printf("DISABLE AUTO EXP SUCCESS.\n");
        } else {
            printf("DISABLE AUTO EXP FAILED.\n");
            return status;
        }
        CameraSdkStatus status1 = CameraSetExposureTime(hCamera, exp_time);
        if (status1 == CAMERA_STATUS_SUCCESS) {
            printf("SET EXP TIME SUCCESS.\n");
        } else {
            printf("SET EXP TIME FAILED.\n");
            return status;
        }
    }
    CameraGetCapability(hCamera,&tCapability);


    return 0;
}

double MVCamera::GetExposureTime()
{
    int auto_exp;
    if (CameraGetAeState(hCamera, &auto_exp) == CAMERA_STATUS_SUCCESS) {
        if (auto_exp) {
            return 0;
        } else {
            double exp_time;
            if (CameraGetExposureTime(hCamera, &exp_time) == CAMERA_STATUS_SUCCESS) {
                return exp_time;
            } else {
                printf("GET CAMERA EXP TIME ERROR.\n");
                return -1;
            }
        }
    } else {
        printf("GET CAMERA AE STATE ERROR.\n");
        return -1;
    }

}

int MVCamera::SetResolution(int resolution_idx, int height, int width, int offset_H, int offset_W)
{
    tSdkImageResolution sRoiResolution;

    //use preset resolution
    if(resolution_idx ==0 || resolution_idx == 1)
    {
        sRoiResolution.iIndex = resolution_idx;
        if (CameraSetImageResolution(hCamera, &sRoiResolution) == CAMERA_STATUS_SUCCESS)
        {
            printf("CAMERA SET RESOLUTION SUCCESS.\n");
        } 
        else
        {
            printf("CAMERA SET  RESOLUTION FAILED.\n");
            return -1;
        }
    }
    else //自定义
    {
        // 设置成0xff表示自定义分辨率，设置成0到N表示选择预设分辨率
        // Set to 0xff for custom resolution, set to 0 to N for select preset resolution
        sRoiResolution.iIndex = 0xff;
        
        // iWidthFOV表示相机的视场宽度，iWidth表示相机实际输出宽度
        // 大部分情况下iWidthFOV=iWidth。有些特殊的分辨率模式如BIN2X2：iWidthFOV=2*iWidth，表示视场是实际输出宽度的2倍
        // iWidthFOV represents the camera's field of view width, iWidth represents the camera's actual output width
        // In most cases iWidthFOV=iWidth. Some special resolution modes such as BIN2X2:iWidthFOV=2*iWidth indicate that the field of view is twice the actual output width

        sRoiResolution.iWidth = width;
        sRoiResolution.iWidthFOV = width;
        
        // 高度，参考上面宽度的说明
        // height, refer to the description of the width above
        sRoiResolution.iHeight = height;
        sRoiResolution.iHeightFOV =height;
        
        // 视场偏移
        // Field of view offset
        sRoiResolution.iHOffsetFOV = offset_W;
        sRoiResolution.iVOffsetFOV = offset_H;
        
        // ISP软件缩放宽高，都为0则表示不缩放
        // ISP software zoom width and height, all 0 means not zoom
        sRoiResolution.iWidthZoomSw = 0;
        sRoiResolution.iHeightZoomSw = 0;
        
        // BIN SKIP 模式设置（需要相机硬件支持）
        // BIN SKIP mode setting (requires camera hardware support)
        sRoiResolution.uBinAverageMode = 0;
        sRoiResolution.uBinSumMode = 0;
        sRoiResolution.uResampleMask = 0;
        sRoiResolution.uSkipMode =  0;


        if (CameraSetImageResolution(hCamera, &sRoiResolution) == CAMERA_STATUS_SUCCESS)
        {
            printf("CAMERA SET RESOLUTION SUCCESS.\n");
        } 
        else
        {
            printf("CAMERA SET RESOLUTION FAILED.\n");
            return -1;
        }
    }

    //初始化缓冲区
    g_pRgbBuffer[0] = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);
    g_pRgbBuffer[1] = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);


    CameraGetCapability(hCamera,&tCapability);

    return 0;
}
// white balance
int MVCamera::SetWBMode(bool auto_wb)
{
    int status = CameraSetWbMode(hCamera, auto_wb);
    if (CAMERA_STATUS_SUCCESS == status) {
        printf("CAMERA SETWBMODE %d SUCCESS!\n", auto_wb);
    } else {
        printf("CAMERA SETWBMODE %d FAILED! ERROR CODE: %d\n", auto_wb, status);
    }
}

int MVCamera::GetWBMode(bool &auto_wb)
{
    int res = 0;
    if (CAMERA_STATUS_SUCCESS == CameraGetWbMode(hCamera, &res)) {
        printf("CAMERA GETWBMODE %d SUCCESS!\n", res);
    } else {
        printf("CAMERA GETWBMODE FAILED!\n");
    }
    auto_wb = res;
}

int MVCamera::SetOnceWB()
{
    int status = CameraSetOnceWB(hCamera);
    if (CAMERA_STATUS_SUCCESS == status) {
        printf("CAMERA SETONCEWB SUCCESS!\n");
    } else {
        printf("CAMERA SETONCEWB FAILED, ERROR CODE: %d!\n", status);
    }
}

int MVCamera::SetGain(double r_gain, double b_gain)
{
    int status = CameraSetGain(hCamera, 100, 100, 100);
    if (CAMERA_STATUS_SUCCESS == status) {
        printf("CAMERA SETGAIN SUCCESS!\n");
    } else {
        printf("CAMERA SETGAIN FAILED! ERROR CODE: %d\n", status);
    }
    CameraGetCapability(hCamera,&tCapability);

}

int  MVCamera::SetDefault()
{
    int status = CameraLoadParameter(hCamera,PARAMETER_TEAM_DEFAULT);
    if (CAMERA_STATUS_SUCCESS == status) 
    {
        printf("CAMERA SET SATURATION SUCCESS!\n");
    } else {
        printf("CAMERA SET SATURATION FAILED! ERROR CODE: %d\n", status);
    }
    CameraGetCapability(hCamera,&tCapability);
}

double MVCamera::GetGain()
{
    int r_gain, g_gain, b_gain;
    int status = CameraGetGain(hCamera, &r_gain, &g_gain, &b_gain);
    if (CAMERA_STATUS_SUCCESS == status) {
        printf("CAMERA GETGAIN SUCCESS!\n");
    } else {
        printf("CAMERA GETGAIN FAILED! ERROR CODE: %d\n", status);
    }

    return (r_gain + g_gain + b_gain)/300.;
}

int MVCamera::Play()
{
    //获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
    CameraGetCapability(hCamera,&tCapability);
    CameraPlay(hCamera);

    //    std::thread thread1(MVCamera::Read);
    //    thread1.detach();
}

// do not use
int MVCamera::Read()
{
    while (!stopped) {
        if (CameraGetImageBuffer(hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS) {
            CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer[(ready_buffer_id+1)%2], &sFrameInfo);
            //            if (iplImage)
            //            {
            //                cvReleaseImageHeader(&iplImage);
            //            }
            //            iplImage = cvCreateImageHeader(cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight),IPL_DEPTH_8U,1);

            //            cvSetData(iplImage,pbyBuffer,sFrameInfo.iWidth);//此处只是设置指针，无图像块数据拷贝，不需担心转换效率

            ready_buffer_id = (ready_buffer_id + 1)%2;
            updated = true;
            CameraReleaseImageBuffer(hCamera, pbyBuffer);
        }
    }
}
int MVCamera::SetFPS(int fps_mode)
{
  int status=CameraSetFrameSpeed(hCamera,fps_mode);
  if(status==CAMERA_STATUS_SUCCESS)
  {
  printf("CAMERA SET FPS MODE SUCCESS! current mode is %d \n" ,fps_mode);
  }else
  {
    printf("CAMERA SET FPS MODE FAILED! ERROR CODE: %d\n", status);
    return -1;// the problem that camera failed to grab img often occurs when in high speed mode(USB3)

  }
  CameraGetCapability(hCamera,&tCapability);
}
///
/// \brief MVCamera::GetFrame_B
/// \param frame
/// \param is_color
/// true if we want bgr img
/// \return
///
int MVCamera::GetFrame(Mat &frame,bool is_color)
{
    if (!stopped) 
    {
        if (CameraGetImageBuffer(hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS) {
          // the problem that camera failed to grab img often occurs when in high speed mode(USB3.0)

            // if (frame.cols != sFrameInfo.iWidth || frame.rows != sFrameInfo.iHeight) {
            //     printf("GetFrame: resize frame !\n");
            //     if(is_color)
            //     {
            //         frame.create(sFrameInfo.iHeight, sFrameInfo.iWidth, CV_8UC3);

            //     }else
            //     {
            //         frame.create(sFrameInfo.iHeight, sFrameInfo.iWidth, CV_8UC1);
            //     }
            // }

            // if (iplImage)
            // {
            //   // delete old one
            //     cvReleaseImageHeader(&iplImage);
            // }
            // iplImage = cvCreateImageHeader(cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight),IPL_DEPTH_8U,1);

            CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer[0],&sFrameInfo);
            frame= Mat(
					cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight),
					sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
					g_pRgbBuffer[0]
					);

        // cvSetData(iplImage,pbyBuffer,sFrameInfo.iWidth);//此处只是设置指针，无图像块数据拷贝，不需担心转换效率
        // Mat Iimag=cvarrToMat(iplImage);//这里只是进行指针转换，将IplImage转换成Mat类型
        // cv::cvtColor(Iimag,frame,COLOR_BayerGR2BGR);


            CameraReleaseImageBuffer(hCamera, pbyBuffer);
        }


    }

}
// do not use
int MVCamera::GetFrameEx(Mat &frame,bool is_color)
{
    //init iplImage
    // if we change resolution
    if (frame.cols != sFrameInfo.iWidth || frame.rows != sFrameInfo.iHeight) {
        printf("GetFrame: resize frame !\n");
        if(is_color)
        {
            frame.create(sFrameInfo.iHeight, sFrameInfo.iWidth, CV_8UC3);

        }else
        {
            frame.create(sFrameInfo.iHeight, sFrameInfo.iWidth, CV_8UC1);
        }
    }

    while (!updated) {
        usleep(1000);
    }
    if(is_color)
    {
        memcpy(frame.data, g_pRgbBuffer[ready_buffer_id], frame.cols*frame.rows*3);
        //        Mat Iimag=cvarrToMat(iplImage);//这里只是进行指针转换，将IplImage转换成Mat类型
        //        cv::cvtColor(Iimag,frame,CV_BayerGR2BGR);

    }
    else
    {
        memcpy(frame.data, g_pRgbBuffer[ready_buffer_id], frame.cols*frame.rows);

    }
    updated = false;
    return 0;
}

int MVCamera::Stop()
{
    stopped = true;
    usleep(30000);
    return 0;
}
