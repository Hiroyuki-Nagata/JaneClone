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
#ifndef JANECLONE_H
#define JANECLONE_H

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

// 自作クラスのヘッダ
#include "ExtractBoardList.h"
#include "SocketCommunication.h"
#include "DataType.h"
#include "VirtualBoardListCtrl.h"
#include "MetakitAccessor.h"
#include "JaneCloneUtil.h"
#include "VirtualBoardListCtrl.h"
#include "ThreadContentWindow.h"
#include "AnchoredResponsePopup.h"

/*
 * 定数値の宣言
 */
#define APP_CONFIG_FILE wxT("janeclone.env")
// 板一覧情報ファイルのパス
static const wxString BOARD_LIST_PATH = wxT("./dat/boardlist.html");
// 板一覧情報ファイルのヘッダ情報のパス
static const wxString BOARD_LIST_HEADER_PATH =
		wxT("./dat/boardlistheader.html");

class JaneClone: public wxFrame {

public:
	// begin wxGlade: JaneClone::ids
	// end wxGlade

	JaneClone(wxWindow* parent, int id, const wxString& title,
			const wxPoint& pos = wxDefaultPosition, const wxSize& size =
					wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
	~JaneClone();

	// 板名とそのURLを保持するwxHashMap　JaneCloneが起動している間は保持される
	// URLvsBoardNameのHashMap（板名をkeyとしてBoardURLとascii文字の固有名を持つ）
	WX_DECLARE_HASH_MAP( wxString, // type of the keys
			URLvsBoardName,// type of the values
			wxStringHash ,// hasher
			wxStringEqual,// key equality predicate
			NameURLHash);// name of the class
	// HashMapの本体
	NameURLHash retainHash;

private:
	// begin wxGlade: JaneClone::methods
	// イベントテーブル系
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnGetBoardList(wxCommandEvent& event);
	void OnVersionInfo(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
	void OnLeftClickAtListCtrl(wxListEvent& event);
	void OnChangedTab(wxAuiNotebookEvent& event);
	void OnRightClickBoardNoteBook(wxAuiNotebookEvent& event);
	void OnRightClickThreadNoteBook(wxAuiNotebookEvent& event);
	void OnCellHover(wxHtmlCellEvent& event);
	void OnCellClicked(wxHtmlCellEvent& event);
	void OnLinkClicked(wxHtmlLinkEvent& event);

	// 各種GUI上の設定
	void SetJaneCloneManuBar();
	void SetProperties();
	void DoLayout();
	void SetJaneCloneAuiPaneInfo();
	void SetPreviousUserLookedTab();

	// 取得した板一覧ファイルからデータを抽出してレイアウトに反映するメソッド
	void SetBoardList();

	// すべてのウィジェットが載るAuiマネージャー
	wxAuiManager m_mgr;
	// ステータスバー表示用文字列
	wxStatusBar* statusBarStr;

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
	// ツリーコントロールは内部からならいじれるようにしておく
	wxTreeCtrl* m_tree_ctrl;
	wxTreeItemData* m_treeData;
	wxTreeItemId m_rootId;

	// ログ出力画面
	wxTextCtrl* m_logCtrl;

	//　ツリーコントロールにクリックした時のイベント
	void OnGetBoardInfo(wxTreeEvent& event);

	/**
	 * 右上のオブジェクトとメソッド
	 */
	// 板名のツリーコントロールをクリックした場合表示されるwxNoteBook
	wxAuiNotebook* boardNoteBook;
	// 板一覧のツリーをクリックして、それをノートブックに反映するメソッド
	void SetBoardNameToNoteBook(wxString& boardName, wxString& boardURL,
			wxString& boardNameAscii);
	// ノートブックに反映する際のコールバック
	void SetThreadListItemNew(const wxString boardName,
			const wxString outputPath, const size_t selectedPage);
	void SetThreadListItemUpdate(const wxString boardName,
			const wxString outputPath, const size_t selectedPage);

	// VirtualBoardListCtrlのHashMap（板名をkeyとしてリストコントロールのオブジェクトを管理する）
	WX_DECLARE_HASH_MAP( wxString, // type of the keys
			VirtualBoardListCtrl,// type of the values
			wxStringHash ,// hasher
			wxStringEqual,// key equality predicate
			VirtualBoardListCtrlHash);// name of the class
	// JaneCloneが管理するBoardTabAndThreadHashのオブジェクト
	VirtualBoardListCtrlHash vbListCtrlHash;

	// VirtualBoardListのHashMap（板名をkeyとしてリストコントロール内部のリストを管理する）
	WX_DECLARE_HASH_MAP( wxString, // type of the keys
			VirtualBoardList,// type of the values
			wxStringHash ,// hasher
			wxStringEqual,// key equality predicate
			VirtualBoardListHash);// name of the class

	VirtualBoardListHash vbListHash;

	/**
	 * 右下のオブジェクトとメソッド
	 */
	// 板名のツリーコントロールをクリックした場合表示されるwxNoteBook
	wxAuiNotebook* threadNoteBook;
	// スレッド一覧をクリックすると、それをスレ表示画面に反映するメソッド
	void SetThreadContentToNoteBook(const wxString&, const wxString&,
			const wxString&);

	// ThreadContentWindowのHashMap（板名をkeyとしてリストコントロールのオブジェクトを管理する）
	WX_DECLARE_HASH_MAP( wxString, // type of the keys
			ThreadContentWindow*, // ポインタを詰める
			wxStringHash , // hasher
			wxStringEqual, // key equality predicate
			ThreadContentWindowHash); // name of the class

	// JaneCloneが管理するBoardTabAndThreadHashのオブジェクト
	ThreadContentWindowHash tcwHash;

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

	// 現在使用しているフォントの情報を取得する
	wxFont GetCurrentFont();

DECLARE_EVENT_TABLE()
};
// wxGlade: end class

#endif // JANECLONE_H
