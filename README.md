# Superchat



# TODO:
- [x] look at our requirements and evaluate
  * [x] requirements table too broad
  * [x] user specific requirements
  * [x] use cases should be based on actions and what the system does, not things in the system.
- [x] **project design**
  * [x] figure out github
  * [x] learn ncurses
  * [x] learn asio
  * [ ] learn readline
    * [x] tab completion
  * [x] class diagram w/ methods and attributes
  * [x] for each class, provide a written description
  * [x] ""   ""   ""  , list if client, server, or both
  * [x] update requirements table
- [ ] code
  * [ ] start writing the classes from UML Diagram.
  * [ ] makefile
  * [ ] echo: do not send back messages to the client sending the messages
  * [ ] ncurses windows with the asio examples
  * [ ] implenting more chat_sessions for chat rooms
  * [ ] read commands
- [ ] testing requirements and report
- [ ] presentation


GIT INSTRUCTIONS:
1. Open Git Bash or whatever terminal your going to use.
	-if your not using Git Bash make sure you have git installed

2. Change your directory to wherever you would like the github to saved, for me this is just under Users/katar
	git init //creates new subdirectory named .git
	So now I have Users/katar/.git

3. git clone https://github.com/kitteacos/Software-Project

4. Add your identity to be able to access the github
	git config --global user.email "your email you used for github here"
	git config --global user.name "whatever you want your name to be"

5. Change the current working directory to your local project. //now named Software-Project

6. git pull //to update master branch from online repository

7. Check out the branch you wish to merge to. Usually, you will merge into master.
	git checkout kat //changes branches
	git checkout master //goes back to master

8. Once inside your branch
	git pull origin master //this pulls from the online master branch

9. Work on anything you want and save

10. git add . //to add the files that are ready to commit

11. git commit -m "Message here for what you are commiting"

12. git push origin [YOUR BRANCH] //pushes updates to your branch on the online respository

13. Once you are confident with all of your work, then go online, into your branch.
You should see a message that says "This branch is x commit/s ahead of mater" then press Pull Request.

14. Review your pull request, change any names and add any comments you would like. Then press create pull request

15. You will then see a message that says [USER] wants to merge x commit/s to master from [YOUR BRANCH] scroll down and press merge pull request, confirm merge.

16. Now your branch is merged with the master branch and everybody can pull these updates to their branch.

17. Each time you code follow steps 5 - 16
