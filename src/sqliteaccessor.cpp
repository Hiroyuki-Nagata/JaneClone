/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012-2014 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#include "sqliteaccessor.hpp"

// dbファイルの初期化
std::function<void(DB&)> SQLiteAccessor::AutoCloseable(std::function<void(DB&)> block) {

    return [&](DB& db) -> void {
        try {
            wxString dbFile = GetDBFilePath();
            DB::InitializeSQLite();
            db.Open(dbFile);
            db.Begin();

            // 実際の処理を実行
            block(db);

            db.Commit();
            db.Close();
        } catch (wxSQLite3Exception& e) {
            wxMessageBox(e.GetMessage());
        }
    };
}

/**
 * データベースの初期化、トランザクションあり
 */
SQLiteAccessor::SQLiteAccessor() {

    DB db;
    AutoCloseable([&](DB& db)
    {
        // 2chのすべての板一覧情報
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS board_info ("
                             "    boardname_kanji TEXT,"
                             "	  board_url		  TEXT,"
                             "	  category		  TEXT,"
                             "    is_outside      INTEGER" /** true:1, false:0 */
                             ");"));

#ifdef USE_SHINGETSU
        // ユーザーが登録している新月のノード情報
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS board_info_shingetsu ("
                             "    timeinfo  TIMESTAMP,"
                             "    board_URL TEXT"
                             ");"));
#endif /** USE_SHINGETSU */

        // ユーザーが前回見ていたタブについての情報
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_looking_boardlist ("
                             "    boardname_kanji TEXT);"));
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_looking_threadlist ("
                             "    thread_title    TEXT,"
                             "    thread_orig_num TEXT,"
                             "    boardname_ascii TEXT"
                             ");"));
        // ユーザーが最近閉じたタブについての情報
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_closed_boardlist ("
                             "    timeinfo        TIMESTAMP,"
                             "    boardname_kanji TEXT,"
                             "    board_url       TEXT,"
                             "    boardname_ascii TEXT"
                             ");"));
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_closed_threadlist ("
                             "    timeinfo TIMESTAMP,"
                             "    thread_title    TEXT,"
                             "    thread_orig_num TEXT,"
                             "    boardname_ascii TEXT"
                             ");"));
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_favorite_threadlist ("
                             "    timeinfo TIMESTAMP,"
                             "    thread_title    TEXT,"
                             "    thread_orig_num TEXT,"
                             "    boardname_ascii TEXT"
                             ");"));
        // ユーザーが検索したキーワードを保存する(板一覧ツリー、スレッド一覧リスト、スレッド表示ウィンドウでの検索)
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_searched_boardname ("
                             "    timeinfo TIMESTAMP,"
                             "    keyword  TEXT"
                             ");"));
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_searched_threadname ("
                             "    timeinfo TIMESTAMP,"
                             "    keyword  TEXT"
                             ");"));
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS user_searched_threadcontents ("
                             "    timeinfo TIMESTAMP,"
                             "    keyword  TEXT"
                             ");"));
        // 画像データの保存先
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS cached_image ("
                             "    timeinfo      TIMESTAMP,"
                             "    filename      TEXT,"
                             "    uuid_filename TEXT"
                             ");"));
    })(db);
}

/**
 * 板一覧情報のコミットを行う
 */
void SQLiteAccessor::SetBoardInfoCommit(wxArrayString* boardInfoArray) {

    DB db;
    AutoCloseable([&](DB& db)
    {
        // 既存のデータは消しておく
        const wxString sqlDel = wxT("DELETE FROM board_info;");
        auto stmt1 = db.PrepareStatement(sqlDel);
        stmt1.ExecuteUpdate();

        // 板一覧更新では外部板情報はコミットされない
        const wxString sqlIn = wxT("INSERT INTO board_info ("
                                   "    BOARDNAME_KANJI,"
                                   "    BOARD_URL,"
                                   "    CATEGORY,"
                                   "    IS_OUTSIDE)"
                                   "VALUES (?, ?, ?, '0');");

        auto stmt2 = db.PrepareStatement(sqlIn);

        for (unsigned int i = 0; i < boardInfoArray->GetCount(); i += 3) {
            // レコードを追加する
            stmt2.ClearBindings();
            stmt2.Bind(1, boardInfoArray->Item(i));
            stmt2.Bind(2, boardInfoArray->Item(i+1));
            stmt2.Bind(3, boardInfoArray->Item(i+2));
            stmt2.ExecuteUpdate();
        }
    })(db);
}

/**
 * 板一覧情報をSQLite内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString SQLiteAccessor::GetBoardInfo() {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    // リザルトセットをArrayStringに設定する
    wxArrayString array;

    try {
        // dbファイルの初期化
        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        const wxString sqlSe = wxT("SELECT BOARDNAME_KANJI, BOARD_URL, CATEGORY FROM BOARD_INFO");

        // SQL文を実行する
        rs = db.ExecuteQuery(sqlSe);
        db.Close();

        while (!rs.Eof()) {
            wxString boardName = rs.GetAsString(wxT("BOARDNAME_KANJI"));
            wxString url = rs.GetAsString(wxT("BOARD_URL"));
            wxString category = rs.GetAsString(wxT("CATEGORY"));

            // 各項目がNULLで無ければArrayStringに詰める
            if (boardName.Length() > 0 && url.Length() > 0 && category.Length() > 0) {
                array.Add(boardName);
                array.Add(url);
                array.Add(category);
            }
            rs.NextRow();
        }
        return array;

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    return array;
}

/**
 * 板一覧情報(カテゴリ一覧)をSQLite内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString SQLiteAccessor::GetCategoryList() {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    // リザルトセットをArrayStringに設定する
    wxArrayString array;

    try {
        // dbファイルの初期化
        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        const wxString sqlSe = wxT("SELECT DISTINCT CATEGORY FROM BOARD_INFO");

        // SQL文を実行する
        rs = db.ExecuteQuery(sqlSe);
        db.Close();

        while (!rs.Eof()) {
            wxString category = rs.GetAsString(wxT("CATEGORY"));

            // 各項目がNULLで無ければArrayStringに詰める
            if (category.Length() > 0) {
                array.Add(category);
            }
            rs.NextRow();
        }
        return array;

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    return array;
}

/**
 * 指定されたテーブルに情報が存在するかどうか聞く(トランザクション処理なし単独)
 */
bool SQLiteAccessor::TableHasData(const wxString tableName) {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();

    try {

        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        // SQL文を用意する
        wxString SQL_QUERY = wxT("SELECT COUNT(*) from ") + tableName;

        // SQL文を実行する
        rs = db.ExecuteQuery(SQL_QUERY);
        db.Close();

        // SQL文を実行し結果を受け取る
        if (!rs.IsNull(0)) {
            int recordNum = rs.GetInt(0);
            if ( 0 == recordNum) {
                return false;
            } else {
                return true;
            }
        }

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    // ここまで来てしまうとエラーなのでfalse
    return false;
}

/**
 * 指定されたテーブルを削除する
 */
void SQLiteAccessor::DropTable(const wxString tableName) {

    DB db;
    AutoCloseable([&](DB& db)
    {
        const wxString sql = wxT("DROP TABLE ?");
        wxSQLite3Statement stmt = db.PrepareStatement (sql);
        stmt.Bind(1, tableName);
    })(db);
}

/**
 * 指定されたテーブルのデータを削除する
 */
void SQLiteAccessor::DeleteTableData(const wxString tableName) {

    DB db;
    AutoCloseable([&](DB& db)
    {
        const wxString sql = wxT("DELETE FROM ") + tableName;
        wxSQLite3Statement stmt = db.PrepareStatement(sql);
        stmt.ExecuteUpdate();
    })(db);
}

/**
 * ユーザーがJaneClone終了時にタブで開いていた板の名前を登録する
 */
void SQLiteAccessor::SetUserLookingBoardList(wxArrayString& userLookingBoardListArray) {

    // Tableの中身を削除する
    DeleteTableData(wxT("user_looking_boardlist"));

    DB db;
    AutoCloseable([&](DB& db)
    {
        // PreparedStatementを準備する
        const wxString sqlIn = wxT("INSERT INTO user_looking_boardlist ("
                                   "    boardname_kanji"
                                   ") VALUES (?);");
        wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

        // 配列内のレコードを追加する
        for (unsigned int i = 0; i < userLookingBoardListArray.GetCount();i++) {
            // レコードを追加する
            stmt.ClearBindings();
            stmt.Bind(1, userLookingBoardListArray.Item(i));
            stmt.ExecuteUpdate();
        }
    })(db);
}
/**
 * JaneClone開始時に以前ユーザーがタブで開いていた板の名前を取得する
 */
wxArrayString SQLiteAccessor::GetUserLookedBoardList() {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxSQLite3Database::InitializeSQLite();
    wxSQLite3Database db;
    db.Open(dbFile);

    // リザルトセットを用意する
    wxSQLite3ResultSet rs;
    // SQL文を用意する
    wxString sqlSe = wxT("SELECT BOARDNAME_KANJI from USER_LOOKING_BOARDLIST");

    // SQL文を実行する
    rs = db.ExecuteQuery(sqlSe);
    db.Close();

    // リザルトセットをArrayStringに設定する
    wxArrayString array;

    while (rs.NextRow()) {
        wxString boardName = rs.GetAsString(wxT("BOARDNAME_KANJI"));

        // 各項目がNULLで無ければArrayStringに詰める
        if (boardName.Length() > 0) {
            array.Add(boardName);
        }
    }
    return array;
}
/**
 * ユーザーがJaneClone終了時にタブで開いていたスレッドの情報を登録する
 */
void SQLiteAccessor::SetUserLookingThreadList(wxArrayString& userLookingThreadListArray) {

    // Tableの中身を削除する
    DeleteTableData(wxT("user_looking_threadlist"));

    DB db;
    AutoCloseable([&](DB& db)
    {
        // PreparedStatementを準備する
        const wxString sqlIn
            = wxT("INSERT INTO user_looking_threadlist ("
                  "    thread_title,"
                  "    thread_orig_num,"
                  "    boardname_ascii"
                  ") VALUES (?, ?, ?);");
        auto stmt = db.PrepareStatement(sqlIn);

        // 配列内のレコードを追加する
        for (unsigned int i = 0; i < userLookingThreadListArray.GetCount();i+=3) {
            // レコードを追加する
            stmt.ClearBindings();
            stmt.Bind(1, userLookingThreadListArray.Item(i));
            stmt.Bind(2, userLookingThreadListArray.Item(i+1));
            stmt.Bind(3, userLookingThreadListArray.Item(i+2));
            stmt.ExecuteUpdate();
        }
    })(db);
}

/**
 * JaneClone開始時に以前ユーザーがタブで開いていたスレッドの情報を取得する
 */
wxArrayString SQLiteAccessor::GetUserLookedThreadList() {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxSQLite3Database::InitializeSQLite();
    wxSQLite3Database db;
    db.Open(dbFile);

    // リザルトセットを用意する
    wxSQLite3ResultSet rs;
    // SQL文を用意する
    wxString sqlSe = wxT("SELECT THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII from USER_LOOKING_THREADLIST");

    // SQL文を実行する
    rs = db.ExecuteQuery(sqlSe);
    db.Close();

    // リザルトセットをArrayStringに設定する
    wxArrayString array;

    while (rs.NextRow()) {
        wxString title = rs.GetAsString(wxT("THREAD_TITLE"));
        wxString origNumber = rs.GetAsString(wxT("THREAD_ORIG_NUM"));
        wxString boardNameAscii = rs.GetAsString(wxT("BOARDNAME_ASCII"));

        // 各項目がNULLで無ければArrayStringに詰める
        if (title.Len() > 0 && origNumber.Len() > 0 && boardNameAscii.Len() > 0) {
            array.Add(title);
            array.Add(origNumber);
            array.Add(boardNameAscii);
        }
    }
    return array;
}

/**
 * ユーザーがスレッドの情報をお気に入りに登録する
 */
void SQLiteAccessor::SetUserFavoriteThreadList(wxArrayString& userFavoriteThreadListArray) {

    // Tableの中身を削除する
    DeleteTableData(wxT("user_favorite_threadlist"));

    DB db;
    AutoCloseable([&](DB& db)
    {
        const wxString sqlIn
            = wxT("INSERT INTO user_favorite_threadlist ("
                  "    thread_title,"
                  "    thread_orig_num,"
                  "    boardname_ascii"
                  ") VALUES (?, ?, ?);");
        wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

        // 配列内のレコードを追加する
        for (unsigned int i = 0; i < userFavoriteThreadListArray.GetCount();i+=3) {
            // レコードを追加する
            stmt.ClearBindings();
            stmt.Bind(1, userFavoriteThreadListArray.Item(i));
            stmt.Bind(2, userFavoriteThreadListArray.Item(i+1));
            stmt.Bind(3, userFavoriteThreadListArray.Item(i+2));
            stmt.ExecuteUpdate();
        }
    })(db);
}
/**
 * ユーザーがお気に入りに登録しているスレッドの情報を取得する
 */
void SQLiteAccessor::GetUserFavoriteThreadList(std::vector<std::tuple<wxString, wxString, wxString>>& favoriteList) {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxSQLite3Database::InitializeSQLite();
    wxSQLite3Database db;
    db.Open(dbFile);

    // リザルトセットを用意する
    wxSQLite3ResultSet rs;
    // SQL文を用意する
    wxString sqlSe = wxT("select THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII ");
    sqlSe += wxT("from USER_FAVORITE_THREADLIST ");
    sqlSe += wxT("order by timeinfo desc");

    // SQL文を実行する
    rs = db.ExecuteQuery(sqlSe);
    db.Close();

    // リザルトセットをvectorに設定する
    while (rs.NextRow()) {
        wxString title = rs.GetAsString(wxT("THREAD_TITLE"));
        wxString origNumber = rs.GetAsString(wxT("THREAD_ORIG_NUM"));
        wxString boardNameAscii = rs.GetAsString(wxT("BOARDNAME_ASCII"));

        // 各項目がNULLで無ければvectorに詰める
        if (title.Len() > 0 && origNumber.Len() > 0 && boardNameAscii.Len() > 0) {
            favoriteList.push_back(std::make_tuple(title, origNumber, boardNameAscii));
        }
    }
}

/**
 * スレタブの情報をSQLiteに格納する
 */
void SQLiteAccessor::SetThreadInfo(ThreadInfo* t, const wxWindowID id) {

    // 現在時間timestamp
    wxDateTime now = wxDateTime::Now();

    // テーブル名の決定
    wxString tableName = wxEmptyString;
    if ( id == wxID_ANY) {
        tableName = wxT("user_closed_threadlist");
    } else if ( id == ID_AddThreadFavorite) {
        tableName = wxT("user_favorite_threadlist");
    } else if ( id == ID_AddBoardFavorite) {
        tableName = wxT("user_favorite_threadlist");
    } else {
        // ERROR
        return;
    }

    DB db;
    AutoCloseable([&](DB& db)
    {
        /** 閉じられたスレタブの情報をインサート */
        const wxString sqlIn =
            wxString::Format("INSERT INTO %s (", tableName)
            + wxT("    timeinfo,"
                  "    thread_title,"
                  "    thread_orig_num,"
                  "    boardname_ascii)"
                  "VALUES (?,?,?,?);");

        auto stmt = db.PrepareStatement(sqlIn);
        stmt.ClearBindings();
        stmt.BindTimestamp(1, now);
        stmt.Bind(2,t->title);
        stmt.Bind(3,t->origNumber);
        stmt.Bind(4,t->boardNameAscii);
        stmt.ExecuteUpdate();
    })(db);
}

/**
 * 最近閉じた板タブ名リストを取得する
 */
wxArrayString SQLiteAccessor::GetClosedBoardInfo() {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxArrayString array;

    try {

        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        // SQL文を用意する
        wxString SQL_QUERY = wxT("select BOARDNAME_KANJI from USER_CLOSED_BOARDLIST order by timeinfo desc limit 16");

        // SQL文を実行する
        rs = db.ExecuteQuery(SQL_QUERY);
        db.Close();

        while (rs.NextRow()) {
            wxString boardName = rs.GetAsString(wxT("BOARDNAME_KANJI"));

            // 各項目がNULLで無ければArrayStringに詰める
            if (boardName.Length() > 0) {
                array.Add(boardName);
            }
        }

        return array;

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    return array;
}

/**
 * 板タブを閉じた際に情報をSQLiteに格納する
 */
void SQLiteAccessor::SetClosedBoardInfo(URLvsBoardName* hash) {

    // ユーザが閉じた板のうち、データベースに保存されている数
    int userClosedBoardListNum = SQLiteAccessor::HowManyRecord(wxT("USER_CLOSED_BOARDLIST"));
    // 現在時間timestamp
    wxDateTime now = wxDateTime::Now();

    DB db;
    AutoCloseable([&](DB& db)
    {
        /** 閉じられたスレタブの情報をインサート */
        const wxString sqlIn =
            wxT("INSERT INTO user_closed_boardlist ("
                "    timeinfo,"
                "    boardname_kanji,"
                "    board_url,"
                "    boardname_ascii)"
                "VALUES (?,?,?,?);");
        wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);
        stmt.ClearBindings();
        stmt.BindTimestamp(1, now);
        stmt.Bind(2,hash->boardName);
        stmt.Bind(3,hash->boardURL);
        stmt.Bind(4,hash->boardNameAscii);
        stmt.ExecuteUpdate();
    })(db);
}

/**
 * 最近閉じたスレタブ名リストを取得する
 */
wxArrayString SQLiteAccessor::GetThreadInfo(const wxWindowID id) {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxArrayString array;

    // テーブル名の決定
    wxString tableName;
    if (id == wxID_ANY) {
        tableName = wxT("USER_CLOSED_THREADLIST");
    } else if (id == ID_UserFavoriteThreadClick) {
        tableName = wxT("USER_FAVORITE_THREADLIST");
    } else {
        // ERROR
        return array;
    }

    try {

        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        // SQL文を用意する
        wxString SQL_QUERY = wxT("SELECT THREAD_TITLE from ")
            + tableName
            + wxT(" order by timeinfo desc limit 16");

        // SQL文を実行する
        rs = db.ExecuteQuery(SQL_QUERY);
        db.Close();

        while (rs.NextRow()) {
            wxString title = rs.GetAsString(wxT("THREAD_TITLE"));

            // 各項目がNULLで無ければArrayStringに詰める
            if (title.Length() > 0) {
                array.Add(title);
            }
        }

        return array;

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    return array;
}

/**
 * 最近閉じたスレッドタブ情報を取得する
 */
void SQLiteAccessor::GetThreadFullInfo(const int number, std::unique_ptr<ThreadInfo>& threadInfo, const wxWindowID id) {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxArrayString array;

    // テーブル名の決定
    wxString tableName;
    if (1000 <= id && id < 1100) {
        tableName = wxT("USER_CLOSED_THREADLIST");
    } else if (1200 <= id && id < 1300) {
        tableName = wxT("USER_FAVORITE_THREADLIST");
    } else {
        // ERROR
        return;
    }

    try {

        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        // SQL文を用意する
        const wxString SQL_QUERY =
            wxT("SELECT THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII from ")
            + tableName
            + wxT(" order by timeinfo desc")
            + wxT(" limit 1 offset ")
            + wxString::Format(wxT("%d"), number);

        // SQL文を実行する
        rs = db.ExecuteQuery(SQL_QUERY);
        db.Close();

        while (rs.NextRow()) {
            threadInfo->title          = rs.GetAsString(wxT("THREAD_TITLE"));
            threadInfo->origNumber     = rs.GetAsString(wxT("THREAD_ORIG_NUM"));
            threadInfo->boardNameAscii = rs.GetAsString(wxT("BOARDNAME_ASCII"));
        }

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }
}

/**
 * 指定されたテーブルにレコードが何件存在するかどうかを調べるメソッド
 */
int SQLiteAccessor::HowManyRecord(const wxString tableName) {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();

    try {

        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        // SQL文を用意する
        wxString SQL_QUERY = wxT("SELECT COUNT(*) from ") + tableName;

        // SQL文を実行する
        rs = db.ExecuteQuery(SQL_QUERY);
        db.Close();

        // SQL文を実行し結果を受け取る
        if (!rs.IsNull(0)) {
            int recordNum = rs.GetInt(0);
            return recordNum;
        }

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    return 0;
}

/**
 * JaneCloneが使用するSQLiteのDBファイルの場所を返す
 * 環境変数"JANECLONE_DB_FILE_PATH"が設定されていればその値を返す
 */
wxString SQLiteAccessor::GetDBFilePath() {

    wxString dbFileFullPath = wxEmptyString;
    bool hasEnvVal = wxGetEnv(
        wxT("JANECLONE_DB_FILE_PATH"), &dbFileFullPath
	);
    if (hasEnvVal) {
        return dbFileFullPath;
    }

    wxFileName dbFile = wxFileName::DirName(::wxGetHomeDir());
    dbFile.AppendDir(JANECLONE_DIR);
    dbFile.SetFullName(SQLITE_FILE_NAME);
    return dbFile.GetFullPath();
}

/**
 * ユーザーが検索ボックスで検索したキーワードをSQLiteに格納する
 */
void SQLiteAccessor::SetUserSearchedKeyword(const wxString& keyword, const wxWindowID id) {

    // 現在時間timestamp
    wxDateTime now = wxDateTime::Now();
    wxString boardName;

    if (id == ID_BoardSearchBarCombo) {
        boardName = wxT("user_searched_boardname");
    } else if (id == ID_ThreadSearchBarCombo) {
        boardName = wxT("user_searched_threadname");
    } else {
        // ERROR
        return;
    }

    DB db;
    AutoCloseable([&](DB& db)
    {
        const wxString sqlIn =
            wxString::Format("INSERT INTO %s (", boardName)
            + wxT("timeinfo, keyword) VALUES (?,?)");

        auto stmt = db.PrepareStatement (sqlIn);
        stmt.ClearBindings();

        stmt.BindTimestamp(1, now);
        stmt.Bind(2, keyword);
        stmt.ExecuteUpdate();
    })(db);
}

/**
 * ユーザーが検索ボックスで検索したキーワードを取得する
 */
wxArrayString SQLiteAccessor::GetUserSearchedKeyword(const wxWindowID id) {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxArrayString array;

    try {

        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        wxString boardName;

        if (id == ID_BoardSearchBarCombo) {
            boardName = wxT("USER_SEARCHED_BOARDNAME");
        } else if (id == ID_ThreadSearchBarCombo) {
            boardName = wxT("USER_SEARCHED_THREADNAME");
        } else {
            // エラールート
            db.Close();
            return array;
        }

        // SQL文を用意する
        const wxString SQL_QUERY = wxT("SELECT KEYWORD from ") + boardName;

        // SQL文を実行する
        rs = db.ExecuteQuery(SQL_QUERY);
        db.Close();

        while (rs.NextRow()) {
            wxString keyword = rs.GetAsString(wxT("KEYWORD"));

            // 各項目がNULLで無ければArrayStringに詰める
            if (keyword.Length() > 0) {
                array.Add(keyword);
            }
        }

        return array;

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    return array;
}

/**
 * 登録済みの新月の公開ノード一覧を取得する
 */
wxArrayString SQLiteAccessor::GetShingetsuNodeList() {

    // dbファイルの初期化
    wxString dbFile = GetDBFilePath();
    wxArrayString array;

    try {

        wxSQLite3Database::InitializeSQLite();
        wxSQLite3Database db;
        db.Open(dbFile);

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        // SQL文を用意する
        const wxString SQL_QUERY = wxT("SELECT BOARD_URL from BOARD_INFO_SHINGETSU");

        // SQL文を実行する
        rs = db.ExecuteQuery(SQL_QUERY);
        db.Close();

        while (rs.NextRow()) {
            array.Add(rs.GetAsString(wxT("BOARD_URL")));
        }

    } catch (wxSQLite3Exception& e) {
        wxMessageBox(e.GetMessage());
    }

    return array;
}

/**
 * 新月の公開ノードを登録する
 */
void SQLiteAccessor::SetShingetsuNode(const wxString& nodeURL) {

    // 現在時間timestamp
    wxDateTime now = wxDateTime::Now();

    DB db;
    AutoCloseable([&](DB& db)
    {
        /** 同じURLが登録されているか */

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;
        // SQL文を用意する
        const wxString sqlSe =
            wxT("SELECT"
                "    COUNT(board_url) "
                "FROM"
                "    board_info_shingetsu "
                "WHERE board_url = ?;");

        auto stmt2 = db.PrepareStatement(sqlSe);
        stmt2.ClearBindings();
        stmt2.Bind(1, nodeURL);
        rs = stmt2.ExecuteQuery();

        int record = 0;

        if (!rs.IsNull(0)) record = rs.GetInt(0);
        if (record == 0) {
            wxString sqlIn = wxT("insert into BOARD_INFO_SHINGETSU values(?, ?)");
            wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

            // レコードを追加する
            stmt.ClearBindings();
            stmt.BindTimestamp(1, now);
            stmt.Bind(2, nodeURL);
            stmt.ExecuteUpdate();
        }
    })(db);
}

/**
 * ダウンロードした画像のファイル名とUUIDをデータベースに格納する
 */
void SQLiteAccessor::SetImageFileName(std::vector<ImageFileInfo>& imageFileInfo)
{
    wxDateTime now = wxDateTime::Now();

    DB db;
    AutoCloseable([&](DB& db)
    {
        for (auto it = imageFileInfo.begin(); it != imageFileInfo.end(); it++) {
            const wxString sqlIn =
                wxT("INSERT INTO cached_image ("
                    "    timeinfo, filename, uuid_filename)"
                    "VALUES (?,?,?);");
            auto stmt = db.PrepareStatement(sqlIn);
            stmt.ClearBindings();

            stmt.BindTimestamp(1, now);
            stmt.Bind(2, it->fileName);
            stmt.Bind(3, it->uuidFileName);
            stmt.ExecuteUpdate();
        }
    })(db);
}

/**
 * ダウンロードした画像のファイル名とUUIDをデータベースに格納する
 * @param const ImageFileInfo* imageFileInfo ファイル情報
 */
void SQLiteAccessor::SetImageFileName(ImageFileInfo& imageFileInfo)
{
    wxDateTime now = wxDateTime::Now();

    DB db;
    AutoCloseable([&](DB& db)
    {
        const wxString sqlIn =
            wxT("INSERT INTO cached_image ("
                "    timeinfo,"
                "    filename,"
                "    uuid_filename)"
                "VALUES (?,?,?);");
        auto stmt = db.PrepareStatement(sqlIn);
        stmt.ClearBindings();

        stmt.BindTimestamp(1, now);
        stmt.Bind(2, imageFileInfo.fileName);
        stmt.Bind(3, imageFileInfo.uuidFileName);
        stmt.ExecuteUpdate();
    })(db);
}

/**
 * 画像のファイル名とUUIDのリストをデータベースから取得する
 * true : データあり
 * false: データなし
 */
bool SQLiteAccessor::GetImageFileName(const wxArrayString& fileNameArray, std::vector<ImageFileInfo>& imageFileInfoArray)
{
    wxSQLite3Database db;
    wxDateTime now = wxDateTime::Now();

    INITIALIZE_JC_WXSQLITE3(db, now)

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;

    for (unsigned int i = 0; i < fileNameArray.GetCount(); i++) {
        const wxString sqlSe = wxT("select UUID_FILENAME from CACHED_IMAGE where FILENAME = ? ");
        wxSQLite3Statement stmt = db.PrepareStatement (sqlSe);
        stmt.ClearBindings();

        stmt.Bind(1, fileNameArray[i]);
        rs = stmt.ExecuteQuery();

        ImageFileInfo iFile;
        iFile.fileName = fileNameArray[i];

        while (rs.NextRow()) {
            iFile.uuidFileName = rs.GetAsString(wxT("UUID_FILENAME"));
        }

        imageFileInfoArray.push_back(iFile);
    }

    CLOSE_CONN_JC_WXSQLITE3(db)

        if (imageFileInfoArray.size() != 0 ) {
            return true;
        } else {
            return false;
        }
}

/**
 * 画像のファイル名とUUIDをデータベースから取得する
 * @param  const wxString&	  画像ファイル名
 * @param  ImageFileInfo>&	  ファイル情報
 * @return true: データあり false: データなし
 */
bool SQLiteAccessor::GetImageFileName(const wxString& fileName, ImageFileInfo& imageFileInfo)
{
    wxSQLite3Database db;
    wxDateTime now = wxDateTime::Now();

    INITIALIZE_JC_WXSQLITE3(db, now)

        // リザルトセットを用意する
        wxSQLite3ResultSet rs;

    // SQL文を用意する
    const wxString SQL_QUERY = wxT("select UUID_FILENAME from CACHED_IMAGE where FILENAME = ? ");

    // SQL文を実行する
    wxSQLite3Statement stmt = db.PrepareStatement (SQL_QUERY);
    rs = stmt.ExecuteQuery();

    while (rs.NextRow()) {
        wxString uuidFileName = rs.GetAsString(wxT("UUID_FILENAME"));

        if (uuidFileName.Length() > 0) {
            imageFileInfo.fileName = fileName;
            imageFileInfo.uuidFileName = uuidFileName;
        }
    }

    CLOSE_CONNONLY_JC_WXSQLITE3(db)

        if ( imageFileInfo.uuidFileName != wxEmptyString ) {
            return true;
        } else {
            return false;
        }
}
/**
 * 外部板の情報をテーブルに登録する
 * @param const wxString& 外部板のURL
 * @param const wxString& 外部板の板名
 * @param const wxString& 外部板のカテゴリ
 */
bool SQLiteAccessor::SetOutSideBoardInfo(const wxString& url,const wxString& boardName,const wxString& category)
{
    DB db;
    AutoCloseable([&](DB& db)
    {
        // SQL文を用意する
        const wxString sqlIn = wxT("INSERT INTO board_info ("
                                   "    boardname_kanji,"
                                   "    board_url, "
                                   "    category, "
                                   "    is_outside"
                                   ") VALUES (?, ?, ?, '1');");

        // SQL文を実行する
        auto stmt = db.PrepareStatement (sqlIn);
        stmt.Bind(1, boardName);
        stmt.Bind(2, url);
        stmt.Bind(3, category);
        stmt.ExecuteUpdate();
    })(db);

    return true;
}
