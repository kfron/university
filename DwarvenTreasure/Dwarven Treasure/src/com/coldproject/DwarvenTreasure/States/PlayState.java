package com.coldproject.DwarvenTreasure.States;

import com.coldproject.DwarvenTreasure.Main.GamePanel;
import com.coldproject.DwarvenTreasure.Map.Map;
import com.coldproject.DwarvenTreasure.HUD.Hud;
import com.coldproject.DwarvenTreasure.Management.GSManager;
import com.coldproject.DwarvenTreasure.Management.KeysHandler;
import com.coldproject.DwarvenTreasure.Entity.Player;
import com.coldproject.DwarvenTreasure.Entity.GoldNugget;
import com.coldproject.DwarvenTreasure.Entity.Graal;
import com.coldproject.DwarvenTreasure.Entity.Item;

import java.awt.image.BufferedImage;
import java.awt.Graphics2D;
import java.util.ArrayList;

public class PlayState extends GameState
{

	private Map map;

	private Player player;

	private Item pickaxe;

	private ArrayList<GoldNugget> gold;

	private Graal graal;


	private Hud hud;
	// sektor - obecna "plansza" na której znajduje sie gracz
	private int xmapSector;
	private int ymapSector;
	private int sectorSize;

	public PlayState(GSManager gsm)
	{
		super(gsm);
	}

	public void init()
	{
		gold = new ArrayList<GoldNugget>();

		map = new Map();
		map.loadTiles("map.png");
		map.loadMap("map.txt");

		player = new Player(map);

		spawnGold();

		player.settotalGold(gold.size());

		spawnItem();

		graal = new Graal(map);
		graal.setTilePosition(4, 4);

		player.setTilePosition(3,11);

		hud = new Hud(player, gold);

		sectorSize = GamePanel.WIDTH;
		xmapSector = player.getx() / sectorSize;
		ymapSector = player.gety() / sectorSize;
		map.setInitialPosition(-xmapSector * sectorSize, -ymapSector * sectorSize);
	}

	public void spawnGold()
	{
		GoldNugget g;

		g = new GoldNugget(map);
		g.setTilePosition(2,19);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(5,26);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(4,36);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(12,19);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(13,29);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(6,10);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(19,12);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(20,21);
		gold.add(g);

		g = new GoldNugget(map);
		g.setTilePosition(28,20);
		gold.add(g);

	}

	public void spawnItem()
	{
		pickaxe = new Item(map);
		pickaxe.setTilePosition(28,38);
	}

	public void update()
	{
		if(player.goldAmmount() == player.totalGold())
			gsm.setState(GSManager.OVER);
		processInput();

		int oldxs = xmapSector;
		int oldys = ymapSector;

		xmapSector = player.getx() / sectorSize;
		ymapSector = player.gety() / sectorSize;
		map.setPosition(-xmapSector * sectorSize, -ymapSector * sectorSize);
		map.update();

		if(map.isMoving()) return;

		player.update();

		graal.update();

		if(player.intersects(graal))
			{
				player.collectedGraal();
			}

		if(player.intersects(pickaxe))
		{
			pickaxe.collected(player);
		}

		for(int i = 0; i < gold.size(); i++)
		{
			GoldNugget g = gold.get(i);
			g.update();

			if(player.intersects(g))
			{
				gold.remove(i);
				i--;

				player.collectedGold();


			}
		}

	}

	public void draw(Graphics2D g)
	{
		map.draw(g);

		graal.draw(g);

		player.draw(g);

		for(GoldNugget gn : gold)
		{
			gn.draw(g);
		}

		if(!pickaxe.isCollected())
			pickaxe.draw(g);

		hud.draw(g);
	}

	public void processInput()
	{
		if(KeysHandler.isDown(KeysHandler.DOWN)) player.setDown();
		if(KeysHandler.isDown(KeysHandler.RIGHT)) player.setRight();
		if(KeysHandler.isDown(KeysHandler.LEFT)) player.setLeft();
		if(KeysHandler.isDown(KeysHandler.UP)) player.setUp();
		if(KeysHandler.isDown(KeysHandler.SPACE)) player.setAction();
	}
	
}