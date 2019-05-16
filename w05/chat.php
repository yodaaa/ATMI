<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<title>phpチャット</title>
</head>
<body bgcolor="#ffffff">
<h1>phpチャット</h1>

<?php

/* データベースの設定 */

$dsn = 'mysql:dbname=chat;host=localhost;charset=utf8'; // データソース
$user = 'chat'; // データベースアクセスのユーザ名
$password = 'chat'; // データベースアクセスのパスワード

/* データベース接続 */

try{
    $db = new PDO($dsn, $user, $password); // インスタンス作成
}catch (PDOException $e){ // エラー処理
    print('データベースに接続できません'.$e->getMessage()); 
    die();
}

/* 投稿があった場合、データベースにデータを挿入 */

if(!empty($_POST['mesg'])){

  if(empty($_POST['name'])) $_POST['name'] = '名無しさん'; // 名前がなければ「名無しさん」になる

  $sql = 'insert into log (date, name, mesg) values (?, ?, ?)';
  $prep = $db->prepare($sql); // プリペアードステートメントとして扱う
  $result = $prep->execute(array(time(),$_POST['name'], $_POST['mesg'])); // プリペアードステートメントは execute メソッドを使う。

  if(!$result){ // エラー処理
   print("データが保存できません:");
   die();
  }
}

/* COOKIE 処理 */

/* フォーム表示 */

?>

<form name="chat_form" method="POST" action="chat.php">
名前：<input type="text" size=10 name="name">
メッセージ：<input type="text" size=30 name="mesg">
<input type="submit" value="送信"><br>
</form><hr>

<?php

date_default_timezone_set('Asia/Tokyo'); // date の出力を日本時間にセット

/* 過去の投稿一覧 */

$sql = 'SELECT * FROM log ORDER BY date DESC LIMIT 20'; // 投稿時間の降順にソート、表示行数を 20 行に限定

$query = $db->query($sql); // プリペアードステートメントを使わなければ query が楽

while($result = $query->fetch(PDO::FETCH_ASSOC)){// $result は連想配列形式
 print("<p>".date("m/d H:i:s", $result['date'])." <b>[ ".$result['name']." ]</b>:".$result['mesg']."</p>\n");// 投稿を一行ずつ表示
}

?>
</body>
</html>
