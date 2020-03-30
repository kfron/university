package com.coldproject.DwarvenTreasure.States;


import java.awt.image.BufferedImage;
import java.awt.Graphics2D;

import javax.imageio.ImageIO;
import com.coldproject.DwarvenTreasure.Main.GamePanel;
import com.coldproject.DwarvenTreasure.Management.GSManager;
import com.coldproject.DwarvenTreasure.Management.KeysHandler;
import com.coldproject.DwarvenTreasure.Management.Graphics;

public class MenuState extends GameState
{
	private BufferedImage bg; 	//background
	private BufferedImage pickaxe;
	private BufferedImage fpx;
	int option = 0;

	public MenuState(GSManager gsm)
	{
		super(gsm);
	}

	public void init()
	{
		try
		{
			pickaxe = ImageIO.read(getClass().getResourceAsStream("pickaxe.png"));
			fpx = ImageIO.read(getClass().getResourceAsStream("fpx.png"));
			bg = ImageIO.read(getClass().getResourceAsStream("menuscreen.png"));

		}catch(Exception e)
		{
			e.printStackTrace();
			System.exit(0);
		}



	}
	public void update()
	{
		processInput();
	}
	public void draw(Graphics2D g)
	{
		g.drawImage(bg, 0, 0, null);
		Graphics.drawString(g, "START", 44, 90);
		Graphics.drawString(g, "EXIT", 48, 100);

		if(option == 0) 
		{
			g.drawImage(pickaxe, 36, 90, null);
			g.drawImage(fpx, 85, 90, null);
		}
		else
		{
			g.drawImage(pickaxe, 40, 100, null);
			g.drawImage(fpx, 81, 100, null);
		}
	}
	public void processInput()
	{
		if(KeysHandler.isPressed(KeysHandler.UP))
		{
			option = (option + 1) % 2;
		}
		if(KeysHandler.isPressed(KeysHandler.DOWN))
		{
			option = (option - 1) % 2;
		}
		if(KeysHandler.isPressed(KeysHandler.ENTER))
		{
			select();
		}
	}

	public void select()
	{
		if(option == 0)
			gsm.setState(GSManager.PLAY);
		else
			System.exit(0);
	}

}