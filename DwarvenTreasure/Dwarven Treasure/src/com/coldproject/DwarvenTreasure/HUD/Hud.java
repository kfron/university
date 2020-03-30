package com.coldproject.DwarvenTreasure.HUD;

import com.coldproject.DwarvenTreasure.Entity.Player;
import com.coldproject.DwarvenTreasure.Entity.GoldNugget;
import com.coldproject.DwarvenTreasure.Main.GamePanel;
import com.coldproject.DwarvenTreasure.Management.Graphics;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;

public class Hud
{
	private int yoffset;

	private BufferedImage bar;
	private BufferedImage gold;
	private BufferedImage pickaxe;

	private Player player;

	private int goldAmmount;

	private Font font;
	private Color textColor;

	public Hud(Player p, ArrayList<GoldNugget> gn)
	{
		player = p;
		goldAmmount = gn.size();
		yoffset = GamePanel.HEIGHT;

		bar = Graphics.BAR[0][0];
		gold = Graphics.GOLD[0][0];
		pickaxe = Graphics.ITEM[0][0];

		font = new Font("Arial", Font.PLAIN, 10);
		textColor = new Color(47, 64, 126);
	}

	public void draw(Graphics2D g)
	{
		g.drawImage(bar, 0, yoffset, null);
		g.setColor(textColor);
		g.fillRect(8, yoffset + 6, (int)(28.0 * player.goldAmmount() / goldAmmount), 4);
		g.setColor(textColor);
		g.setFont(font);
		String s = player.goldAmmount() + "/" + goldAmmount;
		Graphics.drawString(g, s, 40, yoffset + 3);
		if(player.goldAmmount() >= 10) g.drawImage(gold, 80, yoffset, null);
		else g.drawImage(gold, 72, yoffset, null);

		if(player.hasPickaxe()) g.drawImage(pickaxe, 112, yoffset, null);
	}
}