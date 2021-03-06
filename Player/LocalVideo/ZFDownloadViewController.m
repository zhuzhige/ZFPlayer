//
//  ZFDownloadViewController.m
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

#import "ZFDownloadViewController.h"
#import "MoviePlayerViewController.h"
#import "ZFPlayer.h"
#import "ZFDownloadingCell.h"
#import "ZFDownloadedCell.h"

@interface ZFDownloadViewController ()<ZFOffLineVideoCellDelegate,UITableViewDataSource,UITableViewDelegate>

@property (weak, nonatomic  ) IBOutlet UITableView    *tableView;
@property (nonatomic, strong) NSMutableArray *downloadObjectArr;
@end

@implementation ZFDownloadViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.navigationController.navigationBarHidden = NO;
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleDefault animated:YES];
    [self initData];
    [self.tableView reloadData];
    
//    NSMutableArray *downloads = [[ZFDownloadManager sharedInstance] getSessionModels].mutableCopy;
//    self.downloadObjectArr = @[].mutableCopy;
//    NSMutableArray *downladed = @[].mutableCopy;
//    NSMutableArray *downloading = @[].mutableCopy;
//    for (ZFSessionModel *obj in downloads) {
//        if ([[ZFDownloadManager sharedInstance] isCompletion:obj.url]) {
//            [downladed addObject:obj];
//        }else {
//            [downloading addObject:obj];
//        }
//    }
//    [self.downloadObjectArr addObject:downladed];
//    [self.downloadObjectArr addObject:downloading];
//    [self.tableView reloadData];
//
//    NSArray *currentDownload = [[ZFDownloadManager sharedInstance] currentDownloads];
//    int i = 0;
//    for (NSString *url in currentDownload) {
//        [[ZFDownloadManager sharedInstance] isFileDownloadingForUrl:url withProgressBlock:^(CGFloat progress, NSString *speed, NSString *remainingTime, NSString *writtenSize, NSString *totalSize) {
//            NSLog(@"progress = %f, speed= %@, remainingTime = %@, writtenSize = %@, totalSize= %@",progress,speed,remainingTime,writtenSize,totalSize);
//            NSIndexPath *indexPath = [NSIndexPath indexPathForRow:i inSection:1];
//            ZFDownloadingCell *cell = (ZFDownloadingCell *)[self.tableView cellForRowAtIndexPath:indexPath];
//            cell.fileNameLabel.text = speed;
//        }];
//        i++;
//    }
//    [self.tableView reloadData];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView.tableFooterView = [UIView new];
    self.tableView.contentInset = UIEdgeInsetsMake(0, 0, -49, 0);
    NSLog(@"%@", NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES));

}

- (void)initData{
    NSMutableArray *downloads = [ZFDownloadObject readDiskAllCache].mutableCopy;
    self.downloadObjectArr = @[].mutableCopy;
    NSMutableArray *downladed = @[].mutableCopy;
    NSMutableArray *downloading = @[].mutableCopy;
    for (ZFDownloadObject *obj in downloads) {
        if (obj.downloadState == ZFDownloadCompleted) {
            [downladed addObject:obj];
        }else {
            [downloading addObject:obj];
        }
    }
    [self.downloadObjectArr addObject:downladed];
    [self.downloadObjectArr addObject:downloading];
    [self.tableView reloadData];
}

#pragma mark - ZFOffLineVideoCellDelegate

- (void)videoDownload:(NSError *)error index:(NSInteger)index strUrl:(NSString *)strUrl {
    
    if (error) { NSLog(@"error = %@",error.userInfo[NSLocalizedDescriptionKey]);}
    ZFDownloadObject * downloadObject = _downloadObjectArr[1][index];
    [downloadObject removeFromDisk];
    [_downloadObjectArr removeObjectAtIndex:index];
    [self.tableView reloadData];
}

- (void)updateDownloadValue:(ZFDownloadObject *)downloadObject index:(NSInteger)index {
    if (downloadObject != nil) {
        ZFDownloadObject * tempDownloadObject = _downloadObjectArr[1][index];
        tempDownloadObject.currentDownloadLenght = downloadObject.currentDownloadLenght;
        tempDownloadObject.totalLenght = downloadObject.totalLenght;
        tempDownloadObject.downloadSpeed = downloadObject.downloadSpeed;
        tempDownloadObject.downloadState = downloadObject.downloadState;
    }
}

- (void)videoDownloadDidFinished
{
    [self initData];
    [self.tableView reloadData];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}
   
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSArray *sectionArray = self.downloadObjectArr[section];
    return sectionArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ZFDownloadObject *downloadObject = self.downloadObjectArr[indexPath.section][indexPath.row];
    if (indexPath.section == 0) {
        ZFDownloadedCell *cell = [tableView dequeueReusableCellWithIdentifier:@"downloadedCell"];
        cell.downloadObject = downloadObject;
        return cell;
    }else if (indexPath.section == 1) {
        ZFDownloadingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"downloadingCell"];
        cell.delegate = self;
        [cell displayCell:downloadObject index:indexPath.row];
        return cell;
    }
    return nil;
    
//    ZFSessionModel *downloadObject = self.downloadObjectArr[indexPath.section][indexPath.row];
//    if (indexPath.section == 0) {
//        ZFDownloadedCell *cell = [tableView dequeueReusableCellWithIdentifier:@"downloadedCell"];
//        cell.sessionModel = downloadObject;
//        return cell;
//    }else if (indexPath.section == 1) {
//        ZFDownloadingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"downloadingCell"];
//
//        return cell;
//    }
//    return nil;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (NSString *)tableView:(UITableView *)tableView titleForDeleteConfirmationButtonForRowAtIndexPath:(NSIndexPath *)indexPath{
    return @"删除";
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

-(void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSMutableArray *downloadArray = _downloadObjectArr[indexPath.section];
    ZFDownloadObject * downloadObject = downloadArray[indexPath.row];
    [[ZFHttpManager shared] cancelDownloadWithFileName:downloadObject.fileName deleteFile:YES];
    [downloadObject removeFromDisk];
    [downloadArray removeObject:downloadObject];
    [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationBottom];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return @[@"下载完成",@"下载中"][section];
}

#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    
    UITableViewCell *cell            = (UITableViewCell *)sender;
    NSIndexPath *indexPath           = [self.tableView indexPathForCell:cell];
    ZFDownloadObject *model          = self.downloadObjectArr[indexPath.section][indexPath.row];
//    ZFSessionModel *model            = self.downloadObjectArr[indexPath.section][indexPath.row];
    NSURL *videoURL                  = [NSURL fileURLWithPath:ZFFileFullpath(model.fileName)];

    MoviePlayerViewController *movie = (MoviePlayerViewController *)segue.destinationViewController;
    movie.videoURL                   = videoURL;
}


@end
