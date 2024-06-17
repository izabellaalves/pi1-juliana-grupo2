import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { Box, Button, Typography, useTheme } from "@mui/material";
import { tokens } from "../../theme";
import Header from "../../components/Header";
import LineChart from "../../components/LineChart";
import GeographyChart from "../../components/GeographyChart";
import StatBox from "../../components/StatBox";
import TimerIcon from '@mui/icons-material/Timer';
import StraightenIcon from '@mui/icons-material/Straighten';
import BatterySaverIcon from '@mui/icons-material/BatterySaver';
import FlashOnIcon from '@mui/icons-material/FlashOn';
import KeyboardArrowDownIcon from '@mui/icons-material/KeyboardArrowDown';

const Dashboard = () => {
  const theme = useTheme();
  const colors = tokens(theme.palette.mode);

  const [mediaData, setMediaData] = useState(null);
  const [realizaData, setRealizaData] = useState(null);

  const [tick, setTick] = useState(0);

  useEffect(() => {
    const intervalId = setInterval(() => {
      setTick(tick => tick + 1);
    }, 1000);

    return () => clearInterval(intervalId);
  }, []);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const mediaResponse = await axios.get('http://localhost:3333/media');
        const realizaResponse = await axios.get('http://localhost:3333/dados');
        setMediaData(mediaResponse.data[0]);
        setRealizaData(realizaResponse.data[0]);
      } catch (error) {
        console.error('Error fetching data', error);
      }
    };

    fetchData();
  }, [tick]); // Dependência adicionada aqui

  return (
    <Box m="20px">
      {/* HEADER */}
      <Box display="flex" justifyContent="space-between" alignItems="center">
        <Header title="CARRINHO DE LINHA" subtitle="PI 1 | Turma 4 | Grupo 2 | Projeto de Algoritmo" />

        <Box>
          <Button
            sx={{
              backgroundColor: colors.blueAccent[700],
              color: colors.grey[100],
              fontSize: "14px",
              fontWeight: "bold",
              padding: "10px 20px",
            }}
          >
            Trajetórias Anteriores
            <KeyboardArrowDownIcon sx={{ ml: "10px" }} />
          </Button>
        </Box>
      </Box>

      {/* GRID & CHARTS */}
      <Box
        display="grid"
        gridTemplateColumns="repeat(12, 1fr)"
        gridAutoRows="140px"
        gap="20px"
      >
        {/* ROW 1 */}
        {mediaData && (
          <>
            {/* DISTANCIA */}
            <Box
              gridColumn="span 2"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${mediaData.mediaTamanhoPercursos} m`}
                subtitle="Distância"
                progress="0.75"
                increase=""
                icon={
                  <StraightenIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
            {/* TEMPO */}
            <Box
              gridColumn="span 2"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${mediaData.mediaTempoPercursos} s`}
                subtitle="Tempo"
                progress="0.50"
                increase=""
                icon={
                  <TimerIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
            {/* AUTONOMIA */}
            <Box
              gridColumn="span 2"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${mediaData.mediaConsumoEnergetico}`}
                subtitle="Autonomia"
                progress="0.30"
                increase=""
                icon={
                  <BatterySaverIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
            {/* CONSUMO ENERGETICO */}
            <Box
              gridColumn="span 2"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${mediaData.mediaConsumoEnergetico} KWh`}
                subtitle="Consumo Energético"
                progress="0.80"
                increase=""
                icon={
                  <FlashOnIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
          </>
        )}
        
        {/* TRAJETORIA */}
        <Box
          gridColumn="span 4"
          gridRow="span 3"
          backgroundColor={colors.primary[400]}
          padding="30px"
        >
          <Typography
            variant="h5"
            fontWeight="600"
            sx={{ marginBottom: "15px" }}
          >
            Trajetória
          </Typography>
          <Box height="200px">
            <GeographyChart isDashboard={true} />
          </Box>
        </Box>

        {/* ROW 2 */}
        {/* VELOCIDADE */}
        <Box
          gridColumn="span 4"
          gridRow="span 2"
          backgroundColor={colors.primary[400]}
        >
          <Box
            mt="25px"
            p="0 30px"
            display="flex "
            justifyContent="space-between"
            alignItems="center"
          >
            <Box>
              <Typography
                variant="h5"
                fontWeight="600"
                color={colors.grey[100]}
              >
                Velocidade Instantânea
              </Typography>
              <Typography
                variant="h3"
                fontWeight="bold"
                color={colors.greenAccent[500]}
              >
                {realizaData ? `${realizaData.velocidadeInstantanea} m/s` : 'Carregando...'}
              </Typography>
            </Box>
          </Box>
          <Box height="250px" m="-20px 0 0 0">
            <LineChart isDashboard={true} />
          </Box>
        </Box>
        {/* ACELERAÇÃO */}
        <Box
          gridColumn="span 4"
          gridRow="span 2"
          backgroundColor={colors.primary[400]}
        >
          <Box
            mt="25px"
            p="0 30px"
            display="flex "
            justifyContent="space-between"
            alignItems="center"
          >
            <Box>
              <Typography
                variant="h5"
                fontWeight="600"
                color={colors.grey[100]}
              >
                Aceleração Instantânea
              </Typography>
              <Typography
                variant="h3"
                fontWeight="bold"
                color={colors.greenAccent[500]}
              >
                {realizaData ? `${realizaData.aceleracaoInstantanea} m/s²` : 'Carregando...'}
              </Typography>
            </Box>
          </Box>
          <Box height="250px" m="-20px 0 0 0">
            <LineChart isDashboard={true} />
          </Box>
        </Box>
      </Box>
    </Box>
  );
};

export default Dashboard;