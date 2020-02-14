<?php

/**
 * HTTP协议状态码
 * @param int $code 状态码
 * @return string 返回状态码对应的内容
 */
function httpsCode($code)
{
    if ($code >= 600) {
        header('HTTP/1.1 404 Not Found');
        return customHttpCode($code);
    }

    $http = array(
        100 => 'HTTP/1.1 100 Continue',
        101 => 'HTTP/1.1 101 Switching Protocols',
        200 => 'HTTP/1.1 200 OK',
        201 => 'HTTP/1.1 201 Created',
        202 => 'HTTP/1.1 202 Accepted',
        203 => 'HTTP/1.1 203 Non-Authoritative Information',
        204 => 'HTTP/1.1 204 No Content',
        205 => 'HTTP/1.1 205 Reset Content',
        206 => 'HTTP/1.1 206 Partial Content',
        300 => 'HTTP/1.1 300 Multiple Choices',
        301 => 'HTTP/1.1 301 Moved Permanently',
        302 => 'HTTP/1.1 302 Found',
        303 => 'HTTP/1.1 303 See Other',
        304 => 'HTTP/1.1 304 Not Modified',
        305 => 'HTTP/1.1 305 Use Proxy',
        307 => 'HTTP/1.1 307 Temporary Redirect',
        400 => 'HTTP/1.1 400 Bad Request',
        401 => 'HTTP/1.1 401 Unauthorized',
        402 => 'HTTP/1.1 402 Payment Required',
        403 => 'HTTP/1.1 403 Forbidden',
        404 => 'HTTP/1.1 404 Not Found',
        405 => 'HTTP/1.1 405 Method Not Allowed',
        406 => 'HTTP/1.1 406 Not Acceptable',
        407 => 'HTTP/1.1 407 Proxy Authentication Required',
        408 => 'HTTP/1.1 408 Request Time-out',
        409 => 'HTTP/1.1 409 Conflict',
        410 => 'HTTP/1.1 410 Gone',
        411 => 'HTTP/1.1 411 Length Required',
        412 => 'HTTP/1.1 412 Precondition Failed',
        413 => 'HTTP/1.1 413 Request Entity Too Large',
        414 => 'HTTP/1.1 414 Request-URI Too Large',
        415 => 'HTTP/1.1 415 Unsupported Media Type',
        416 => 'HTTP/1.1 416 Requested range not satisfiable',
        417 => 'HTTP/1.1 417 Expectation Failed',
        500 => 'HTTP/1.1 500 Internal Server Error',
        501 => 'HTTP/1.1 501 Not Implemented',
        502 => 'HTTP/1.1 502 Bad Gateway',
        503 => 'HTTP/1.1 503 Service Unavailable',
        504 => 'HTTP/1.1 504 Gateway Time-out',
    );
    header($http[$code]);
    return $http[$code];
}

/**
 * 自定义HTTP状态码
 * @param int $code 状态码
 * @return string 返回状态码对应的内容
 *
 * 600 - 649 : 数据库相关状态
 * 650 - 699 : 文件读取相关状态
 * 700 - 799 : 用户登录相关状态
 */
function customHttpCode($code)
{
    $http = array(
        600 => '数据库连接失败',
        650 => '文件解析失败',
        651 => '无法初始化GD图像流',
        700 => '用户数据为空',
        701 => '用户不存在',
        702 => '账号或密码错误'
    );
    return $http[$code];
}

/**
 * 应答请求
 * @param int $code 状态码
 * @param string $message 相关信息
 * @param array $data 应答返回json的数据
 */
function reply($data)
{
    httpsCode(200);
    echo json_encode($data/*, JSON_NUMERIC_CHECK*/);
}

function errorReply($code)
{
    echo json_encode(['code' => $code, 'msg' => httpsCode($code)]);
}
