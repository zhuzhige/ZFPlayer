//
//  ZFHttpManager.h
//
// Copyright (c) 2016年 任子丰 ( http://github.com/renzifeng )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
#import "ZFHttpOperation.h"
#import "ZFDownloadOperation.h"
#import "Reachability.h"

/**
 * 默认下载并发数量
 */

extern const NSInteger kZFDefaultDownloadNumber;

/**
 * 说明: ZFHttpManager http请求管理类 (单例设计模式 支持POST/GET 请求 文件上传 文件下载)
 */

@interface ZFHttpManager : NSObject

/**
 * 说明: 当前是否是等待下载状态
 */
- (BOOL)waitingDownload;

/**
 * 网络管理单例对象
 */
+ (nonnull instancetype)shared;

/**
 * 网络参数编码类型
 */

@property (nonatomic , assign) NSUInteger     encoderType;

/**
 * 网络请求缓存策略
 */

@property (nonatomic , assign) NSURLRequestCachePolicy cachePolicy;

/**
 * 网络请求超时时长
 */

@property (nonatomic , assign) NSTimeInterval timeoutInterval;

/**
 * 网络请求内容类型
 */
@property (nonatomic , copy , nonnull)  NSString *  contentType;

/**
 * 网络请求404错误集合(下次再遇到则不请求)
 */

@property (nonatomic , strong , nullable) NSMutableSet * failedUrls;

/**
 * 网络请求会话id对象
 */
@property (nonatomic , strong , nullable) NSString * cookie;

/**
 * 创建文件保存路径
 * @param: savePath 文件路径
 */
- (BOOL)createFileSavePath:(nonnull NSString *)savePath;

/**
 * 生成通用错误对象
 * @param: message 错误信息
 */

- (nonnull NSError *)error:(nonnull NSString *)message;

/**
 * 自动处理生成正确文件名
 * @param: saveFileName 保存文件名
 * @param: strUrl 下载地址
 */

- (nullable NSString *)handleFileName:(nonnull NSString *)saveFileName url:(nonnull NSString *)strUrl;

/**
 * 当前网络状态
 */

@property (nonatomic , assign)NetworkStatus        networkStatus;


/**
 * 注册监听设备网络状态
 */
- (void)registerNetworkStatusMoniterEvent;

/**
 * GET 请求操作
 * @param: strUrl 请求地址
 * @param: finishedBlock 完成块
 */

- (nullable ZFHttpOperation *)get:(nonnull NSString *)strUrl
               didFinished:(nullable ZFDidFinished)finishedBlock;


/**
 * GET 请求操作
 * @param: strUrl 请求地址
 * @param: processBlock 请求过程块
 * @param: finishedBlock 完成块
 */

- (nullable ZFHttpOperation *)get:(nonnull NSString *)strUrl
                            process:(nullable ZFProgress) processBlock
                        didFinished:(nullable ZFDidFinished)finishedBlock;

/**
 * POST 请求操作
 * @param: strUrl 请求地址
 * @param: param 请求参数
 * @param: finishedBlock 完成块
 */

- (nullable ZFHttpOperation *)post:(nonnull NSString *)strUrl
                               param:(nullable NSString *)param
                         didFinished:(nullable ZFDidFinished)finishedBlock;

/**
 * POST 请求操作
 * @param: strUrl 请求地址
 * @param: param 请求参数
 * @param: processBlock 请求过程块
 * @param: finishedBlock 完成块
 */

- (nullable ZFHttpOperation *)post:(nonnull NSString *)strUrl
                               param:(nullable NSString *)param
                             process:(nullable ZFProgress)processBlock
                         didFinished:(nullable ZFDidFinished)finishedBlock;

/**
 * 文件上传 请求操作
 * @param: strUrl 请求地址
 * @param: param 请求参数
 * @param: finishedBlock 完成块
 */

- (nullable ZFHttpOperation *)upload:(nonnull NSString *)strUrl
                                 param:(nullable NSDictionary *)paramDict
                           didFinished:(nullable ZFDidFinished)finishedBlock;

/**
 * 文件上传 请求操作
 * @param: strUrl 请求地址
 * @param: param 请求参数
 * @param: processBlock 请求过程块
 * @param: finishedBlock 完成块
 */

- (nullable ZFHttpOperation *)upload:(nonnull NSString *)strUrl
                                 param:(nullable NSDictionary *)paramDict
                               process:(nullable ZFProgress)processBlock
                           didFinished:(nullable ZFDidFinished)finishedBlock;



/**
 * 说明: 执行下载任务 (存储时使用默认文件名)
 * @param strUrl 下载地址
 * @param savePath 下载缓存路径
 * @param delegate 下载响应代理
 */

- (nullable ZFDownloadOperation *)download:(nonnull NSString *)strUrl
                                    savePath:(nonnull NSString *)savePath
                                    delegate:(nullable id<ZFDownloadDelegate>)delegate;

/**
 * 说明: 执行下载任务
 * @param strUrl 下载地址
 * @param savePath 下载缓存路径
 * @param saveFileName 下载保存文件名
 * @param delegate 下载响应代理
 */

- (nullable ZFDownloadOperation *)download:(nonnull NSString *)strUrl
                                    savePath:(nonnull NSString *)savePath
                                saveFileName:(nullable NSString *)saveFileName
                                    delegate:(nullable id<ZFDownloadDelegate>)delegate;

/**
 * 说明: 执行下载任务 (存储时使用默认文件名)
 * @param strUrl 下载地址
 * @param savePath 下载缓存路径
 * @param responseBlock 下载响应回调
 * @param processBlock 下载过程回调
 * @param didFinishedBlock 下载完成回调
 */

- (nullable ZFDownloadOperation *)download:(nonnull NSString *)strUrl
                                    savePath:(nonnull NSString *)savePath
                                    response:(nullable ZFResponse)responseBlock
                                     process:(nullable ZFProgress)processBlock
                                 didFinished:(nullable ZFDidFinished)finishedBlock;

/**
 * 说明: 执行下载任务
 * @param strUrl 下载地址
 * @param savePath 下载缓存路径
 * @param saveFileName 下载保存文件名
 * @param responseBlock 下载响应回调
 * @param processBlock 下载过程回调
 * @param didFinishedBlock 下载完成回调
 */

- (nullable ZFDownloadOperation *)download:(nonnull NSString *)strUrl
                                    savePath:(nonnull NSString *)savePath
                                saveFileName:(nullable NSString *)saveFileName
                                    response:(nullable ZFResponse) responseBlock
                                     process:(nullable ZFProgress) processBlock
                                 didFinished:(nullable ZFDidFinished) finishedBlock;




/**
 * 说明: 添加上传文件数据
 * @param: data 文件数据
 * @param: fileName 文件名
 * @param: mimeType 文件类型
 * @param: key 上传标识 (这个必须和服务端对应)
 */

- (void)addUploadFileData:(nonnull NSObject *)data
             withFileName:(nonnull NSString *)fileName
                 mimeType:(nonnull NSString *)mimeType
                   forKey:(nonnull NSString *)key;

/**
 * 说明: 添加上传文件
 * @param: filePath 本地文件路径
 * @param: key 上传标识 (这个必须和服务端对应)
 */

- (void)addUploadFile:(nonnull NSString *)filePath
               forKey:(nonnull NSString *)key;


/**
 * 说明: 取消http请求
 * @param: url http 地址
 */

- (void)cancelHttpRequestWithUrl:(nonnull NSString *)url ;

/**
 * 说明: 返回指定文件名下载对象
 * @param: fileName 下载文件名
 */

- (nullable ZFDownloadOperation *)downloadOperationWithFileName:(nonnull NSString *)fileName;

/**
 * 说明:设置最大下载数量（该方法必须在开始下载之前调用）
 * @param: count 下载并发数量
 */

- (void)setMaxDownloadQueueCount:(NSUInteger)count;

/**
 * 说明:返回下载中心最大同时下载操作个数
 */

- (NSInteger)currentDownloadCount;

/**
 * 说明：取消所有当前下载任务
 * @param isDelete 是否删除缓存文件
 */

- (void)cancelAllDownloadTaskAndDelFile:(BOOL)isDelete;

/**
 * 说明：取消指定正下载url的下载
 * @param isDelete 是否删除缓存文件
 */

- (void)cancelDownloadWithDownloadUrl:(nonnull NSString *)strUrl
                           deleteFile:(BOOL)isDelete;
/**
 * 说明：取消指定正下载文件名的下载
 * @param isDelete 是否删除缓存文件
 */

- (void)cancelDownloadWithFileName:(nonnull NSString *)fileName
                        deleteFile:(BOOL)isDelete;


/**
 * 说明：替换当前回调通过传递要下载的文件名(当从控制器B进入到控制器C然后在控制器C中进行下载，然后下载过程中突然退出到控制器B，在又进入到控制器C，这个时候还是在下载但是代理对象和之前的那个控制器C不是一个对象所以要替换)
 * @param responseBlock 下载响应回调
 * @param processBlock 下载过程回调
 * @param didFinishedBlock 下载完成回调
 * @param fileName 文件名
 */

- (nullable ZFDownloadOperation *)replaceCurrentDownloadOperationBlockResponse:(nullable ZFResponse)responseBlock
                                             process:(nullable ZFProgress)processBlock
                                         didFinished:(nullable ZFDidFinished)didFinishedBlock
                                            fileName:(nonnull NSString *)fileName;

/**
 * 说明：替换当前回调通过传递要下载的文件名(当从控制器B进入到控制器C然后在控制器C中进行下载，然后下载过程中突然退出到控制器B，在又进入到控制器C，这个时候还是在下载但是代理对象和之前的那个控制器C不是一个对象所以要替换)
 * @param delegate 下载回调新代理
 * @param fileName 文件名
 */

- (nullable ZFDownloadOperation *)replaceCurrentDownloadOperationDelegate:(nullable id<ZFDownloadDelegate>)delegate
                                       fileName:(nonnull NSString *)fileName;

/**
 * 说明：替换当前所有下载代理(当从控制器B进入到控制器C然后在控制器C中进行下载，然后下载过程中突然退出到控制器B，在又进入到控制器C，这个时候还是在下载但是代理对象和之前的那个控制器C不是一个对象所以要替换)
 * @param responseBlock 下载响应回调
 * @param processBlock 下载过程回调
 * @param didFinishedBlock 下载完成回调
 */
- (nullable ZFDownloadOperation *)replaceAllDownloadOperationBlockResponse:(nullable ZFResponse)responseBlock
                                         process:(nullable ZFProgress)processBlock
                                     didFinished:(nullable ZFDidFinished)didFinishedBlock;

/**
 * 说明：替换当前所有下载代理(当从控制器B进入到控制器C然后在控制器C中进行下载，然后下载过程中突然退出到控制器B，在又进入到控制器C，这个时候还是在下载但是代理对象和之前的那个控制器C不是一个对象所以要替换)
 * @param delegate 下载回调新代理
 */

- (nullable ZFDownloadOperation *)replaceAllDownloadOperationDelegate:(nullable id<ZFDownloadDelegate>)delegate;

/**
 * 说明：通过要下载的文件名来判断当前是否在进行下载任务
 * @param fileName 正在下载的文件名
 */

- (BOOL)existDownloadOperationTaskWithUrl:(nonnull NSString *)strUrl;


/**
 * 说明：通过要下载的文件名来判断当前是否在进行下载任务
 * @param strUrl 正在下载的url
 */

- (BOOL)existDownloadOperationTaskWithFileName:(nonnull NSString *)fileName;

/**
 * 获取文件名格式通过Url
 * @param: downloadUrl 下载路径
 */

- (nullable NSString *)fileFormatWithUrl:(nonnull NSString *)downloadUrl;

/**
 * 生成http请求原始参数对象
 * @param: paramDictionary 参数字典
 */

- (nonnull NSString*)createHttpParam:(nonnull NSDictionary *)paramDictionary;

@end
