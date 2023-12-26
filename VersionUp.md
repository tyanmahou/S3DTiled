# バージョンアップの手順


- 既存プロジェクトの名前を変える
- 新規プロジェクトS3DTiled追加
  - ソースファイルを移動

- 構成の種類を `.lib` に
- インクルードディレクトリィに追加
```
$(ProjectDir)\include
```

- ビルド後イベント設定
```
xcopy /I /D /Y "$(OutDir)$(TargetFileName)" "$(ProjectDir)lib"
```
- [Debug]のターゲット名を `$(ProjectName)_d` に修正

- ビルドオプション追加
```
/source-charset:utf-8 
```
- Main.cpp をビルド対象から除外
