var http = require( 'http' ); // HTTPモジュール読み込み
var socketio = require( 'socket.io' ); // Socket.IOモジュール読み込み
var fs = require( 'fs' ); // ファイル入出力モジュール読み込み for index.html

// MongoDB データベースの設定
var mongo = require('mongodb'); 
var db = new mongo.Db('chat', new mongo.Server('localhost', 27017, {}), {}); // データベース名は chat 、27017 は MongoDB のデフォルトポート

// 4000番ポートでHTTPサーバーを立てる
var server = http.createServer( function( req, res ) {
    res.writeHead(200, { 'Content-Type' : 'text/html' }); // ヘッダ出力
    res.end( fs.readFileSync('./index.html', 'utf-8') );  // index.htmlの内容を出力
}).listen(4000);

// サーバーをソケットに紐付ける
var io = socketio.listen( server );

// UNIX time を日付文字列に変換
function getDateStr ( unixtime ) {
    
    var tmp_date = new Date(unixtime); // Date オブジェクト取得
    var tmp = {}; // 日付用連想配列
    
    // 月、日、時、分、秒
    tmp['month'] = tmp_date.getMonth() + 1; // getMonth() は 0-11 
    tmp['day'] = tmp_date.getDate();
    tmp['hour'] = tmp_date.getHours();
    tmp['minute'] = tmp_date.getMinutes();
    tmp['second'] = tmp_date.getSeconds();

    // 一桁の場合は 0 を追加
    for(key in tmp){
	if (tmp[key] < 10) tmp[key] = "0" + tmp[key];
    }

    // 文字列を返す
    return tmp['month'] + "/" + tmp['day'] +" "+ tmp['hour'] +":"+ tmp['minute'] +":" + tmp['second'] ;
}

// 接続確立後の処理
io.sockets.on( 'connection', function( socket ) {
    // 投稿をクライアントから取得
    socket.on( 'post_mesg', function( data ) {

	// 現在時の取得
	var now = new Date();
	var nowUtime = now.getTime(); //UNIX timeに変更

	db.open(function() {	// DB 接続
	    db.collection('log', function(err, collection) { // コレクション名は log
		// doc に JSON 形式でデータを代入
		doc = {
		    "date" : nowUtime, // 投稿日時
		    "name" : data.name, // 投稿者名
		    "mesg" : data.mesg // 投稿内容
		};
		collection.insert(doc, function() { // データベースに追加
		    db.close(); // 接続終了
		});
		// 投稿の文字列作成
		var buf = "<p>" + getDateStr(nowUtime) +" <b>[ " + doc.name + " ]</b>" + doc.mesg + "</p>\n"; 

		// サーバーからクライアントへ メッセージを送り返し
		io.sockets.emit('callback', buf );
	    });
	});
    });


    // 過去のリストを取得
    socket.on( 'getList', function() {
	db.open(function() {
	    var buf = ""; // バッファ
	    var cursor = db.collection('log').find().sort({"date": -1}).limit(20); // log の内容を全検索、結果を日付の降順にソートし 20 行に限定
	    cursor.each(function(err, doc){ // 一行ずつ処理
		if(doc != null){　　// データがある場合、文字列をバッファに追加する
		    buf += "<p>" + getDateStr(doc.date) +" <b>[ " + doc.name + " ]</b>" + doc.mesg + "</p>\n";
		}else{ // 検索終了後はクライアントに送信、DB接続を終了
		    io.to(socket.id).emit('callback', buf);
		    db.close();
		}
	    });
	});
    });
});
