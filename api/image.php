<?php
require_once(__DIR__ . '\utility.php');

$user = @$_GET['user'];
$id = @$_GET['id'];
$size = @$_GET['size'];

// 如果是用户
if ($user == 'user') {
    // 如果id存在
    if ($id) {
        header('Content-Type:image/png');
        $imgsrc = 'http://inkchat.com/images/avatar/' . $id . '.png';

        if ($size >= 512) {
            if (!@readfile($imgsrc)) {
                errorReply(404);
                exit;
            }
        } else {
            // 如果要获取的是缩略图，则处理图片为 45*45
            avatarResize($imgsrc, $size);
        }
    }
}

/**
 * 重设头像尺寸
 *
 * @param string $file_name 文件名
 *
 * 包含全路径（可以是相对路径和 web地址）和文件名称
 *
 * @param int $newsize [optional]
 *
 * 新尺寸，尽量`比原尺寸小`，因为该函数会`降低`缩小后图片的质量。另外该方法主要针对`等宽高`的头像图片
 * 所以，参数中不提供分别对宽高设置的内容，即默认`等比例缩放`。
 *
 * @return boolean 如果重设成功则返回 true，否则返回 false
 *
 */
function avatarResize($file_name, $newsize = 45)
{
    // 返回一个数组，第一个元素 (索引值 0) 是图片的高度，单位是像素 (pixel)。
    // 第二个元素 (索引值 1) 是图片的宽度。
    // 第三个元素 (索引值 2) 是图片的文件格式，其值 1 为 GIF 格式、 2 为 JPEG/JPG 格式、3 为 PNG 格式。
    // 第四个元素 (索引值 3) 为图片的高与宽字符串，height=xxx width=yyy。
    $size = @getimagesize($file_name) or (errorReply(404) and die);

    // 声明一个真彩图片资源，此时只是一个有宽高的黑白图片。
    $newimg = @imagecreatetruecolor($newsize, $newsize) or (errorReply(651) and die);

    // 根据图片的格式使用不同的方法创建为新的图片
    if ($size[2] == 1) $src = imagecreatefromgif($file_name);
    if ($size[2] == 2) $src = imagecreatefromjpeg($file_name);
    if ($size[2] == 3) {
        $src = imagecreatefrompng($file_name);

        // 不丢失图像的透明色
        imagesavealpha($src, true);
        // 不合并颜色，直接用原图像颜色替换，包括透明色。
        imagealphablending($newimg, false);
        // 是不要丢了$newimg的透明色
        imagesavealpha($newimg, true);
    }

    // 参数（目标资源，源，目标资源的开始坐标x,y, 源资源的开始坐标x,y,目标资源的宽高w,h,源资源的宽高w,h）返回值为 bool。
    imagecopyresampled($newimg, $src, 0, 0, 0, 0, $newsize, $newsize, $size[1], $size[0]);

    // header('Content-Type: image/png');

    // 将图片塞进去，根据图片不同的格式选用不同的方法，同时降低适当质量。
    if ($size[2] == 1) $result = @imagegif($newimg, null, 6);
    if ($size[2] == 2) $result = @imagejpeg($newimg, null, 6);
    if ($size[2] == 3) $result = @imagepng($newimg, null, 6);

    // 销毁资源
    @imagedestroy($newimg);

    // 返回生成好的图片
    return $result;
}
