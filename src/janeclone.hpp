/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012 Hiroyuki Nagata
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
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Nov 01 01:42:24 2011
#ifndef JANECLONE_HPP_
#define JANECLONE_HPP_

// 純粋なwxWidgetsライブラリはここに
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/statusbr.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/xml/xml.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/mstream.h>
#include <wx/url.h>
#include <wx/protocol/http.h>
#include <wx/datstrm.h>
#include <wx/aboutdlg.h>
#include <wx/hashmap.h>
#include <wx/regex.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/textfile.h>
#include <wx/convauto.h>
#include <wx/process.h>
#include <wx/srchctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/strconv.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/menu.h>

// 自作クラスのヘッダ
#include "enums.hpp"
#include "extractboardlist.hpp"
#include "socketcommunication.hpp"
#include "datatype.hpp"
#include "virtualboardlistctrl.hpp"
#include "sqliteaccessor.hpp"
#include "janecloneutil.hpp"
#include "threadcontentwindow.hpp"
#include "threadcontentbar.hpp"
#include "anchoredresponsepopup.hpp"
#include "responsewindow.hpp"
#include "janecloneimageviewer.hpp"

/**
 * JaneClone本体はGUI構築用のwxFrameと
 * マウスモーション管理用のwxMouseEventsManagerを継承する
 */
class JaneClone : public wxFrame {

public:
     // begin wxGlade: JaneClone::ids
     // end wxGlade

     JaneClone(wxWindow* parent, int id, const wxString& title,
	       const wxPoint& pos = wxDefaultPosition, const wxSize& size =
	       wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
     ~JaneClone();

     // 板名とそのURLを保持するwxHashMap　JaneCloneが起動している間は保持される
     // URLvsBoardNameのHashMap（板名をkeyとしてBoardURLとascii文字の固有名を持つ）
     WX_DECLARE_HASH_MAP( wxString, 	 // type of the keys
			  URLvsBoardName,// type of the values
			  wxStringHash , // hasher
			  wxStringEqual, // key equality predicate
			  NameURLHash);  // name of the class
     // HashMapの本体
     NameURLHash retainHash;
     // 板名のツリーコントロールをクリックした場合表示されるwxNoteBook
     wxAuiNotebook* boardNoteBook;
     // 板名のツリーコントロールをクリックした場合表示されるwxNoteBook
     wxAuiNotebook* threadNoteBook;

     // JaneClone内部のイメージビューアのインスタンス
     static JaneCloneImageViewer* imageViewer;
     static JaneCloneImageViewer* GetJaneCloneImageViewer();
     
     // 再起動処理のためのPID
     unsigned long pid;

private:

     // begin wxGlade: JaneClone::methods
     // イベントテーブル系
     void OnQuit(wxCommandEvent& event);
     void OnAbout(wxCommandEvent& event);
     void OnRestart(wxCommandEvent& event);
     void OnGetBoardList(wxCommandEvent& event);
     void CheckLogDirectory(wxCommandEvent& event);
     void OnVersionInfo(wxCommandEvent& event);
     void FontDialogBoardTree(wxCommandEvent& event);
     void FontDialogLogWindow(wxCommandEvent& event);
     void FontDialogBoardNotebook(wxCommandEvent& event);
     void FontDialogThreadNotebook(wxCommandEvent& event);
     void FontDialogThreadContents(wxCommandEvent& event);
     void SetFontDialog(const int enumType);
     void OnOpenJaneCloneOfficial(wxCommandEvent& event);

     // 動的に要素を確保するメニュー
     wxMenu* closeT;
     wxMenu* closeB;
     wxMenu* lookingTB;
     void OnMenuOpen(wxMenuEvent& event);
     void UserLastClosedThreadMenuUp(wxUpdateUIEvent& event);
     void UserLastClosedBoardMenuUp(wxUpdateUIEvent& event);
     void UserLookingTabsMenuUp(wxUpdateUIEvent& event);
     void UserLookingTabsControl(wxUpdateUIEvent& event);

     // スレッド一覧タブ処理
     void OneBoardTabClose(wxCommandEvent& event);
     void ExcepSelTabClose(wxCommandEvent& event);
     void AllBoardTabClose(wxCommandEvent& event);
     void AllLeftBoardTabClose(wxCommandEvent& event);
     void AllRightBoardTabClose(wxCommandEvent& event);
     void OnOpenBoardByBrowser(wxCommandEvent& event);
     void ReloadOneBoard(wxCommandEvent& event);
     void CopyBURLToClipBoard(wxCommandEvent& event);
     void CopyBTitleToClipBoard(wxCommandEvent& event);
     void CopyBBothDataToClipBoard(wxCommandEvent& event);
     void CallResponseWindow(wxCommandEvent& event);
     
     // スレタブでの処理
     void OneThreadTabClose(wxCommandEvent& event);
     void ExcepSelThreadTabClose(wxCommandEvent& event);
     void AllThreadTabClose(wxCommandEvent& event);
     void AllLeftThreadTabClose(wxCommandEvent& event);
     void AllRightThreadTabClose(wxCommandEvent& event);
     void OnOpenThreadByBrowser(wxCommandEvent& event);
     void CopyTURLToClipBoard(wxCommandEvent& event);
     void CopyTTitleToClipBoard(wxCommandEvent& event);
     void CopyTBothDataToClipBoard(wxCommandEvent& event);
     void SaveDatFile(wxCommandEvent& event);
     void SaveDatFileToClipBoard(wxCommandEvent& event);
     void DeleteDatFile(wxCommandEvent& event);
     void ReloadThisThread(wxCommandEvent& event);
     // いろいろなイベント処理
     void OnCloseWindow(wxCloseEvent& event);
     void OnLeftClickAtListCtrl(wxListEvent& event);
     void OnLeftClickAtListCtrlCol(wxListEvent& event);
     void OnChangeBoardTab(wxAuiNotebookEvent& event);
     void OnChangeThreadTab(wxAuiNotebookEvent& event);
     void OnChangedBoardTab(wxAuiNotebookEvent& event);
     void OnChangedThreadTab(wxAuiNotebookEvent& event);
     void OnRightClickBoardNoteBook(wxAuiNotebookEvent& event);
     void OnRightClickThreadNoteBook(wxAuiNotebookEvent& event);
     void OnAboutCloseThreadNoteBook(wxAuiNotebookEvent& event);
     void OnAboutCloseBoardNoteBook(wxAuiNotebookEvent& event);
     void OnCellHover(wxHtmlCellEvent& event);
     void OnClickURLWindowButton(wxCommandEvent& event);
     void OnSetFocus(wxFocusEvent& event);

     // マウスモーション
     void MotionEnterWindow(wxMouseEvent& event);
     void MotionLeaveWindow(wxMouseEvent& event);

     // 各種GUI上の設定
     void SetJaneCloneManuBar();
     void SetProperties();
     void DoLayout();
     void SetJaneCloneAuiPaneInfo();
     void SetPreviousUserLookedTab();
     void InitializeJaneClone(wxString& jc, wxDir& jcDir);

     // ツリーコントロールが乗るパネル
     wxPanel* m_boardTreePanel;
     // ツリーコントロール
     wxTreeCtrl* m_tree_ctrl;
     // 取得した板一覧ファイルからデータを抽出したのちwxTreeCtrlのインスタンスを返す
     void SetBoardList();
     // 板一覧更ツリーの初期化
     void InitializeBoardList();

     // すべてのウィジェットが載るAuiマネージャー
     wxAuiManager m_mgr;
     // ステータスバー表示用文字列
     wxStatusBar* statusBarStr;

     // マウスクリックしているかどうか
     bool isClicking;
     // ドラッグしているかどうか
     bool isDragging;

     /**
      * 画面上部のオブジェクトとメソッド
      */
     // 検索バー
     wxSearchCtrl* m_search_ctrl;
     // URL入力欄が載るパネル
     wxPanel* m_url_input_panel;
     // URL入力欄
     wxTextCtrl* m_url_input;
     // URL入力欄の画像つきボタン
     wxBitmapButton* m_url_input_button;
     // URLを表すString
     wxString m_url_text;

     /**
      * 画面左側のオブジェクトとメソッド
      */
     // ログ出力画面
     wxTextCtrl* m_logCtrl;

     //　ツリーコントロールにクリックした時のイベント
     void OnGetBoardInfo(wxTreeEvent& event);

     /**
      * 右上のオブジェクトとメソッド
      */
     // 板一覧のツリーをクリックして、それをノートブックに反映するメソッド
     void SetBoardNameToNoteBook(wxString& boardName, wxString& boardURL,
				 wxString& boardNameAscii);
     // ノートブックに反映する際のコールバック
     void SetThreadListItemNew(const wxString boardName,
			       const wxString outputPath, const size_t selectedPage);
     void SetThreadListItemUpdate(const wxString boardName,
				  const wxString outputPath, const size_t selectedPage);

     // VirtualBoardListCtrlのHashMap（板名をkeyとしてリストコントロールのオブジェクトを管理する）
     WX_DECLARE_HASH_MAP( wxString, 		// type of the keys
			  VirtualBoardListCtrl,		// type of the values
			  wxStringHash ,				// hasher
			  wxStringEqual,				// key equality predicate
			  VirtualBoardListCtrlHash);	// name of the class
     // JaneCloneが管理するBoardTabAndThreadHashのオブジェクト
     VirtualBoardListCtrlHash vbListCtrlHash;

     // VirtualBoardListのHashMap（板名をkeyとしてリストコントロール内部のリストを管理する）
     WX_DECLARE_HASH_MAP( wxString, 		// type of the keys
			  VirtualBoardList,			// type of the values
			  wxStringHash ,				// hasher
			  wxStringEqual,				// key equality predicate
			  VirtualBoardListHash);		// name of the class

     VirtualBoardListHash vbListHash;

     // スレッド検索ボックスを隠す
     void HideThreadSearchBar(wxCommandEvent& event);

     /**
      * 右下のオブジェクトとメソッド
      */

     // スレッド一覧をクリックすると、それをスレ表示画面に反映するメソッド
     void SetThreadContentToNoteBook(const wxString&, const wxString&,
				     const wxString&);

     // ユーザーがタブに保持しているスレッドの情報を保存するHashSetの宣言
     WX_DECLARE_HASH_MAP( wxString, 		// type of the keys
			  ThreadInfo,           // 実体を詰める
			  wxStringHash ,        // hasher
			  wxStringEqual,        // key equality predicate
			  ThreadInfoHash);      // name of the class

     // ユーザーがタブに保持しているスレッドの情報を保存するHashSet
     ThreadInfoHash tiHash;

     /**
      * その他のオブジェクトとメソッド
      */
     // スレッド一覧の情報を保持するwxHashMap　ユーザが板名をクリックするたびに作られる
     // ThreadListクラスについてはDataType.h参照
     WX_DECLARE_HASH_MAP( int, ThreadList*, wxIntegerHash, wxIntegerEqual, ThreadListHash );
     // ThreadListHashの本体
     ThreadListHash m_threadListHash;

     // wxFileConfigクラスのインスタンス
     wxFileConfig* config;
     // ポップアップウィンドウを出現させる
     void SetPopUpWindow(wxHtmlCellEvent& event, wxString& boardNameAscii,
			 wxString& origNumber, wxString& resNumber, wxPoint& anchorPoint);
     /**
      *  フォント読み出し系の処理
      */
     // 現在使用しているフォントの情報を取得する
     wxFont GetCurrentFont();
     // フォント情報をコンフィグファイルに書き出す
     void WriteFontInfo(wxWindow* current);
     // ウィジェットの名前ごとにコンフィグファイルから設定を取り出す
     wxFont ReadFontInfo(const wxString& widgetName);

     /**
      *  よく使う処理のコールバック関数
      */
     // スレタブ上に存在するスレッドのURLを返す
     wxString GetThreadURL(const wxString title,const wxString boardNameAscii,const wxString origNumber);
     // スレッド一覧画面にツールバーを設定する
     wxPanel* CreateAuiToolBar(wxWindow* parent, const wxString& boardName, const wxString& outputPath);
     // 検索用ツールバー設定の共通部分
     void CreateCommonAuiToolBar(wxPanel* panel, wxBoxSizer* vbox, wxWindowID id, const wxString& boardName = wxEmptyString);

     DECLARE_EVENT_TABLE()
};
// wxGlade: end class

#endif // JANECLONE_HPP_
