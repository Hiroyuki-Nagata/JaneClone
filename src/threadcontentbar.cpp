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

// -*- C++ -*- generated by wxGlade 0.6.3 on Sat Dec 29 22:07:54 2012

#include "threadcontentbar.hpp"
#include "janeclone.hpp"

// event table
BEGIN_EVENT_TABLE(ThreadContentBar, wxPanel)
// マウスホバー時のイベントテーブル
//EVT_MOUSE_EVENTS(ThreadContentBar::OnHoverTCBAutoReload)
//EVT_BUTTON(ID_TCBAutoReload, ThreadContentBar::OnClickTCBAutoReload)
// EVT_ENTER_WINDOW(ID_TCBRedResExtract, ThreadContentBar::OnClickTCBRedResExtract)
// EVT_ENTER_WINDOW(ID_TCBRefresh, ThreadContentBar::OnClickTCBRefresh)
// EVT_ENTER_WINDOW(ID_TCBScrollToNewRes,  ThreadContentBar::OnClickTCBScrollToNewRes)
// EVT_ENTER_WINDOW(ID_TCBStop,            ThreadContentBar::OnClickTCBStop)
// EVT_ENTER_WINDOW(ID_TCBResExtract,      ThreadContentBar::OnClickTCBResExtract)
// EVT_ENTER_WINDOW(ID_TCBNewThread,       ThreadContentBar::OnClickTCBNewThread)
// EVT_ENTER_WINDOW(ID_TCBResponse,        ThreadContentBar::OnClickTCBResponse)
// EVT_ENTER_WINDOW(ID_TCBBookMark,        ThreadContentBar::OnClickTCBBookMark)
// EVT_ENTER_WINDOW(ID_TCBDeleteLog,       ThreadContentBar::OnClickTCBDeleteLog)
// EVT_ENTER_WINDOW(ID_TCBClose,           ThreadContentBar::OnClickTCBClose)
// EVT_ENTER_WINDOW(ID_TCBNormalSearch,    ThreadContentBar::OnClickTCBNormalSearch)
// EVT_ENTER_WINDOW(ID_TCBHideSearchBar,   ThreadContentBar::OnClickTCBHideSearchBar)
// EVT_ENTER_WINDOW(ID_TCBForward,         ThreadContentBar::OnClickTCBForward)
// EVT_ENTER_WINDOW(ID_TCBBackward,        ThreadContentBar::OnClickTCBBackward)
END_EVENT_TABLE()


ThreadContentBar::ThreadContentBar(wxWindow* parent, int wxWindowID, const wxPoint& pos, const wxSize& size, long style):
wxPanel(parent, wxWindowID, pos, size, wxDEFAULT_FRAME_STYLE)
{
     // begin wxGlade: ThreadContentBar::ThreadContentBar
     threadContentsBarUpperSizer = new wxPanel(this, wxID_ANY);
     threadContentsBarUpperSizer->SetBackgroundColour(*wxLIGHT_GREY);

     // スレッドタイトルを保持する
     threadName = new wxStaticText(threadContentsBarUpperSizer, wxID_ANY, wxEmptyString);
     spacePanel1 = new wxPanel(threadContentsBarUpperSizer, wxID_ANY);

     // wxAuiToolBar1を宣言する
     threadToolbar1 = new wxAuiToolBar(threadContentsBarUpperSizer, wxID_ANY, wxDefaultPosition, wxDefaultSize,
				       wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
     threadToolbar1->SetToolBitmapSize(wxSize(32,32));
     threadToolbar1->AddTool(ID_TCBAutoReload, wxT("autoreload"), 
			     wxBitmap(autoReloadImg, wxBITMAP_TYPE_ANY), 
			     wxT("オートリロード・スクロール"));
     threadToolbar1->AddTool(ID_TCBRedResExtract, wxT("redresextract"), 
			     wxBitmap(redResExtractImg, wxBITMAP_TYPE_ANY), 
			     wxT("赤レス抽出"));
     // 新着チェックボタンは▼ボタンを押すとメニューが出る
     threadToolbar1->AddSeparator();
     threadToolbar1->AddTool(ID_TCBRefresh, wxT("refresh"), 
			     wxBitmap(refreshImg, wxBITMAP_TYPE_ANY), 
			     wxT("新着チェック/表示レス数/スレの再描画"));
     // メニューの設定
     wxAuiToolBarItemArray prepend_items1;
     wxAuiToolBarItemArray append_items1;
     wxAuiToolBarItem item;
     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_ReloadThisThread);
     item.SetLabel(wxT("新着チェック"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(wxID_ANY);
     item.SetLabel(wxT("すべてのタブの新着チェック"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(wxID_ANY);
     item.SetLabel(wxT("全てのタブの更新チェック"));
     append_items1.Add(item);
     threadToolbar1->SetCustomOverflowItems(prepend_items1, append_items1);
     threadToolbar1->Realize();

     // wxAuiToolBar2を宣言する
     threadToolbar2 = new wxAuiToolBar(threadContentsBarUpperSizer, wxID_ANY, wxDefaultPosition, wxDefaultSize,
				       wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
     threadToolbar2->SetToolBitmapSize(wxSize(32,32));
     threadToolbar2->AddTool(ID_TCBScrollToNewRes, wxT("scrolltonewres"), 
			     wxBitmap(scrollToNewResImg, wxBITMAP_TYPE_ANY), 
			     wxT("新着までスクロール"));
     threadToolbar2->AddTool(ID_TCBStop, wxT("stop"), 
			     wxBitmap(stopImg, wxBITMAP_TYPE_ANY), 
			     wxT("中止"));
     threadToolbar2->AddTool(ID_TCBResExtract, wxT("resextract"), 
			     wxBitmap(resExtractImg, wxBITMAP_TYPE_ANY), 
			     wxT("レス抽出"));
     threadToolbar2->AddTool(ID_TCBNewThread, wxT("newthread"), 
			     wxBitmap(newThreadImg, wxBITMAP_TYPE_ANY), 
			     wxT("次スレ候補検索/次スレ候補を開く"));
     threadToolbar2->AddTool(ID_CallResponseWindow, wxT("response"), 
			     wxBitmap(responseImg, wxBITMAP_TYPE_ANY), 
			     wxT("レス"));
     threadToolbar2->AddTool(ID_TCBBookMark, wxT("bookmark"), 
			     wxBitmap(bookMarkImg, wxBITMAP_TYPE_ANY), 
			     wxT("ブックマークに追加"));
     threadToolbar2->AddTool(ID_TCBDeleteLog, wxT("deletelog"), 
			     wxBitmap(deleteLogImg, wxBITMAP_TYPE_ANY), 
			     wxT("ログ削除"));
     threadToolbar2->AddTool(ID_TCBClose, wxT("close"), 
			     wxBitmap(closeImg, wxBITMAP_TYPE_ANY), 
			     wxT("タブを閉じる/新着なしのタブを閉じる"));

     // メニューの設定
     wxAuiToolBarItemArray prepend_items2;
     wxAuiToolBarItemArray append_items2;
     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_OneThreadTabClose);
     item.SetLabel(wxT("このタブを閉じる"));
     append_items2.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(wxID_ANY);
     item.SetLabel(wxT("未読として閉じる"));
     append_items2.Add(item);

     item.SetKind(wxITEM_SEPARATOR);
     append_items2.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_ExcepSelThreadTabClose);
     item.SetLabel(wxT("このタブ以外を閉じる"));
     append_items2.Add(item);

     item.SetKind(ID_AllThreadTabClose);
     item.SetId(wxID_ANY);
     item.SetLabel(wxT("全てのタブを閉じる"));
     append_items2.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_AllLeftThreadTabClose);
     item.SetLabel(wxT("これより左を閉じる"));
     append_items2.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_AllRightThreadTabClose);
     item.SetLabel(wxT("これより右を閉じる"));
     append_items2.Add(item);

     item.SetKind(wxITEM_SEPARATOR);
     append_items2.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(wxID_ANY);
     item.SetLabel(wxT("新着なしのタブを閉じる"));
     append_items2.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(wxID_ANY);
     item.SetLabel(wxT("dat落ちのタブを閉じる"));
     append_items2.Add(item);

     threadToolbar2->SetCustomOverflowItems(prepend_items2, append_items2);
     threadToolbar2->AddSeparator();

     threadToolbar2->Realize();

     // 通常検索
     nomalSearchButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(normalSearchImg, wxBITMAP_TYPE_ANY),
					    wxDefaultPosition, threadContentBarImgSize);

     // スレッド内検索用コンボボックス
     searchWordCombo_choices = NULL;
     searchWordCombo = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, searchWordComboSize, 0, searchWordCombo_choices, wxCB_DROPDOWN);
     searchWordCombo->SetFont(wxFont(12, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));

     // 引っかかった検索ワードを前後させる
     backwardButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(backwardImg, wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize,
					 wxBU_AUTODRAW, wxDefaultValidator, wxT("前へ"));
     forwardButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(forwardImg,wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize,
					 wxBU_AUTODRAW, wxDefaultValidator, wxT("次へ"));
     panel_2 = new wxPanel(this, wxID_ANY);

     // 検索バーを隠す
     hideSearchBarButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(hideSearchBarImg, wxBITMAP_TYPE_ANY),
					      wxDefaultPosition, threadContentBarImgSize);
     // スレッドの内容が乗るパネル
     threadContentPanel = new wxPanel(this, wxID_ANY);

     set_properties();
     do_layout();
     // end wxGlade
}


void ThreadContentBar::set_properties()
{
    // begin wxGlade: ThreadContentBar::set_properties
    threadName->SetFont(wxFont(12, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));

    nomalSearchButton->SetSize(nomalSearchButton->GetBestSize());
    hideSearchBarButton->SetSize(hideSearchBarButton->GetBestSize());
    // end wxGlade
}


void ThreadContentBar::do_layout()
{
    // begin wxGlade: ThreadContentBar::do_layout
    wxBoxSizer* threadContentsBarSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* horizonalSizer2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* horizonalSizer1 = new wxBoxSizer(wxHORIZONTAL);
    horizonalSizer1->Add(threadName, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    horizonalSizer1->Add(spacePanel1, 1, wxEXPAND, 0);

    // スレッド内容バー右上のボタン群
    horizonalSizer1->Add(threadToolbar1, 0, wxALIGN_RIGHT, 0);
    horizonalSizer1->Add(threadToolbar2, 0, wxALIGN_RIGHT, 0);

    threadContentsBarUpperSizer->SetSizer(horizonalSizer1);
    threadContentsBarSizer->Add(threadContentsBarUpperSizer, 0, wxEXPAND, 0);

    horizonalSizer2->Add(nomalSearchButton, 0, 0, 0);
    horizonalSizer2->Add(searchWordCombo, 0, 0, 0);
    horizonalSizer2->Add(backwardButton, 0, 0, 0);
    horizonalSizer2->Add(forwardButton, 0, 0, 0);
    horizonalSizer2->Add(panel_2, 1, wxEXPAND, 0);
    horizonalSizer2->Add(hideSearchBarButton, 0, wxALIGN_RIGHT, 0);
    threadContentsBarSizer->Add(horizonalSizer2, 0, wxEXPAND, 0);
    threadContentsBarSizer->Add(threadContentPanel, 1, wxEXPAND | wxBOTTOM, 5);
    SetSizer(threadContentsBarSizer);
    Layout();
    // end wxGlade
}

void ThreadContentBar::SetTitle(const wxString& title) {
     // スレッドタイトルを設定する
     threadName->SetLabel(wxT("　") + title);
}

void ThreadContentBar::SetThreadContentWindow(const wxString& threadContentPath) {
     // スレッドの内容を表すウィンドウをthreadContentPanelを親として宣言する
     ThreadContentWindow* tcw = new ThreadContentWindow(threadContentPanel, threadContentPath);
     wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
     vbox->Add(tcw, 1, wxEXPAND | wxALL, 5);
     threadContentPanel->SetSizer(vbox);
}

// void ThreadContentBar::OnClickTCBAutoReload(wxCommandEvent& event) {
//      wxMessageBox(wxT("click!"));
// }
// void ThreadContentBar::OnHoverTCBAutoReload(wxMouseEvent& event) {
//      wxMessageBox(wxT("hover!"));
// }
// void ThreadContentBar::OnClickTCBRedResExtract(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBRefresh(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBScrollToNewRes(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBStop(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBResExtract(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBNewThread(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBResponse(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBBookMark(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBDeleteLog(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBClose(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBNormalSearch(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBHideSearchBar(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBForward(wxCommandEvent event) {
// }
// void ThreadContentBar::OnClickTCBBackward(wxCommandEvent event) {
// }
