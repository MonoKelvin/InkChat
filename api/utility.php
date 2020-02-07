<?php
require_once(__DIR__ . '\mysql_api.class.php');

/**
 * 应答请求
 * @param int $code 状态码
 * @param string $message 相关信息
 * @param array $data 应答返回json的数据
 */
function reply($data = null, $code = 200, $message = '')
{
    $res = array('code' => $code, 'msg' => $message, 'data' => $data);
    echo json_encode($res);
}
