###Git is a distributed version control system
##command
git init  - 在当前目录创建库

git add <filename> 向暂存区添加文件

git commit -m "***" 降暂存区内容提交到本地库

git status 显示本地库当前状态

git diff 显示当前工作目录修改的内容，（与暂存区不同的内容）

git diff -- <filename> 查看具体文件的修改内容

git log 查看所有的提交

git log --pretty=oneline 单行显示提交信息

git中HEAD指当前版本 HEAD^上一个版本 HEAD^^上上一个版本 HEAD～3往上3个版本

git reset --hard HEAD^ 回退到上个版本

git reset --hard <版本号> 回退到某个提交的版本

git reflog 记录了每条命令

git diff HEAD -- <filename> 查看当前版本工作区和版本库不同的内容

git checkout -- <filename> 丢弃这个文件工作区的修改，不会丢弃已添加到暂存区的内容

git reset HEAD <filename> 撤销暂存区的修改，放回工作区

git rm <filename> 删除暂存区的文件，等待提交


##Remote Repository

 ssh-keygen -t rsa -C "youremail@example.com" 创建SSH KEY

 git remote add origin git@github.com:michaelliao/learngit.git 本地仓库绑定远程仓库

git push -u origin master 第一次push本地文件

git remote 查看远程库信息

git remote -v 详细信息

git push origin master 推送分支

git branch 查看分支

git checkout -b dev origin/dev 本地创建分支dev，并切换为当前分支，并从远程库上拉去分支

git branch --set-upstream dev origin/dev 将本地dev分支与远程dev分支链接

git branch dev 创建devfenz

git checkout dev 切换到dev分支

git merge dev 将dev分支合并到当前分支

git branch -d dev 删除dev分支

git log --graph命令可以看到分支合并图。

git merge --no-ff -m "merge with no-ff" dev 普通合并，git log可查看分支历史

git stash 将工作目录保存起来，并恢复为仓库的当前版本

git stash list 查看stash列表

git stash pop 恢复之前保存的工作区内容，并将它从stash移除

git branch -D dev 强行删除未合并的分支

git tag v1.0 创建标签v1.0

git tag 查看标签

git tag v0.9 commid—id 给某次提交打上标签

git show <tagname> 查看某标签

git tag -a <tagname> -m "blablabla..."可以指定标签信息；

git tag -s <tagname> -m "blablabla..."可以用PGP签名标签；

git tag -d <tagname> 删除标签

git push origin <tagname> 推送某个标签到远程

git push origin --tags  次性推送全部尚未推送到远程的本地标签

git push origin :refs/tags/v0.9 删除远程标签（本地已删除）

###config
 git config --global alias.st status 配置status别名st

git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"
炫彩log