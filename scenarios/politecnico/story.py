#!/usr/bin/env python

# Variável de referência para Storyboard Omnet++ do módulo: board
import storyboard
import timeline


# Obs: Já que SUMO e OMNeT++ usam sistemas de coordenas diferentes,
# um deslocamento vertical é necessário
maxY_SUMO = 2248.29

print ("story.py successfully imported...")

def createStories(board):

    # Cria coordenadas necessárias para Condição de área poligonal (PolygonCondition)
	# Os vértices são dados em coordenadas do OMNeT++
	coord0 = storyboard.Coord(650.0, maxY_SUMO - 1074.0)
	coord1 = storyboard.Coord(910.0, maxY_SUMO - 964.0)
	coord2 = storyboard.Coord(1020.0, maxY_SUMO - 1217.0)
	coord3 = storyboard.Coord(757.0, maxY_SUMO - 1328.0)

	# Configura o primeiro veículo (nó 0) como alvo do Efeito
	carCond = storyboard.CarSetCondition("flow.0")

	# Cria Condição de Área Poligonal (PolygonCondition) sob a qual o efeito será aplicado
	polyCond = storyboard.PolygonCondition([coord0, coord1, coord2, coord3])

	# Cria Condição lógica AND (AndCondition) entre as condições criadas anteriormente
	andCond = storyboard.AndCondition(carCond, polyCond)

    # Cria efeito sob a velocidade (SpeedEffect) - veículo tem velocidade máxima de 3 mps
	speedEffect = storyboard.SpeedEffect(8)

	# Cria um efeito de envio de mensagem
	signalEffect = storyboard.SignalEffect("Alert on")

    # Cria e registra o Story
	story = storyboard.Story(andCond, [signalEffect, speedEffect])
	board.registerStory(story)

	print("Story loaded!")