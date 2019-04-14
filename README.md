# Superchat



# TODO:
- [ ] code
  * [x] start writing the classes from UML Diagram.
  * [ ] makefile and clean directories
  * [x] echo: do not send back messages to the client sending the messages NOTE:(will fix with ncurses)
  * [x] comment code for Friday
  * [x] finish interface functions
  * [ ] all of client in header? that way we can use the functions inside chat_gui
  * [ ] implement saurav code
    * [ ] turn to ncurses
  * [ ] 9000 is default, 9001 lobby, 9002-9011 is new rooms.
  * [ ] ncurses windows with the asio examples
  * [ ] implenting more chat_sessions for chat rooms
  * [x] read commands
  * [ ] redo UML diagram

- [ ] testing requirements and report
  * [ ] josh and ryan <------
- [ ] presentation


# GIT INSTRUCTIONS:
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
