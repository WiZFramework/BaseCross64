　このたびは、「WiZ（国際情報工科自動車大学校）C++、Dx12ゲームフレームワーク（通称BaseCross64）」GitHubサイト、にアクセスいただきありがとうございます。  

　WiZ（国際情報工科自動車大学校）ホームページ  
　http://www.wiz.ac.jp/index.html   

　「BaseCross64」は「BaseCross」の後継にあたります。（BaseCrossは以下リンクです）  
https://github.com/WiZFramework/BaseCross  

＊「BaseCross64」の開発環境はVisualStudio2017、x64、Windows10SDK（SDKバージョン「10.0.17134.0」以降）です。  
＊「BaseCross」が32ビット（Win32）だったのに対して、「BaseCross64」は64ビット環境となります。  
＊SDKバージョンの「10.0.17134.0」は重要です。  
　このバージョンは2018年5月のWindows10の大きなバージョンアップに合わせたSDKと理解しています。  
　VisualStudio2017のアップデートがおすみでない方は、「ツール」「ツールの機能を取得」でインストーラーを最新の状態にしてからSDKバージョン「10.0.17134.0」もセットアップしてください。  
　とにかく、VisualStudio2017を2018年5月時点での最新の状態にしてください。  

また、ドキュメント部分のみオンライン公開されています。  
https://wizframework.github.io/Documents/  

の「BaseCross64」を参照ください。
ダウンロードする前に内容を確認したい場合などにご利用ください。

  
　DirectX12も、少しずつですが、本家（マイクロソフト社）のサンプルも充実してきて、フレームワークも作成しやすい環境が整いつつあります。  
　ですが、なかなかネイティブで動作するマシンも追いつくのがやっとというのが現状かと思います。  
　ゲーム専用のマシンならともかく、ビジネス用のマシンでは、もう少し時間がかかりそうです。  
  
　というわけで、少し気長にかまえながらフレームワークの作成をはじめたいと思います。  
　とりあえずの目標としましては「BaseCross」の持っている機能は一通り、Dx11、Dx12対応しつつ、「BaseCross」の使いにくい部分を修正していきたいと思います。  
　「BaseCross」が「Dx11の場合はSDK8.1、Dx12の場合はWin10SDK」とちょっと入り組んだ環境だったのですが「BaseCross64」では「Dx11、Dx12いずれもWin10SDKの64ビット」とスッキリしましたので、「Dx12向けのフルバージョン」も実装できるかと思います。  
  
　「BaseCross64」のダウンロード方法は、この文書が表示されているページの、右側にあります、「DownloadZIP」のボタンをクリックして、パソコン等にダウンロードしてください。  
　zipでダウンロードした場合は、zipの状態で「プロパティ」でセキュリティの設定を外してください。こうしないとVisualStudio2017が正常にビルドできない場合があります。  
　もしくはレポジトリをSourceTreeなどでクローンしておくのも手です。こうしておくと、「プロパティ」の設定は必要ないばかりではなく、定期的にPULLしておくことで最新の状態を保つことができます。  
  
　更新や追加情報はfacebookにて紹介いたします。  
　もし更新情報などを知りたければ、facebookにて「YasushiYamanoi」を検索して、  
https://wizframework.github.io/Documents/img/yy.png  
  
　この似顔絵のアイコンを見つけましたら「友達依頼」を出してください。更新した都度、なるべく発言をするようにしています。    
※主な更新履歴です（細かいのは、gitなのでレポジトリ内を参照ください）  
  
2018年5月19日　最初のサンプル実装  
2018年11月5日　基本機能実装（一部Dx11のみ）
  
　なおライセンスは「MITライセンス」となります。改変など自由ですが、くれぐれもご自分の責任においてお使いください。  
　それでは末永くよろしくお願いします。  
  
　　　　　　　　　　　　　　　　　　　　　2018年11月　山ノ井　靖（WiZゲーム科非常勤講師：C++担当）




